#include "Characters/Components/AttributesComponent.h"
#include "Characters/Components/AttributeCore.h"
#include "Net/UnrealNetwork.h"


// ---------------------------
// Конструктор компонента
// ---------------------------
UAttributesComponent::UAttributesComponent()
{	
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bCanEverTick = true;

	TickInterval = 0.5f;
	TimeAccumulator = 0.f;

	Health = nullptr;
	Stamina = nullptr;
	SpiritEnergy = nullptr;
	
	HealthViewModel = nullptr;
	StaminaViewModel = nullptr;
	SpiritViewModel = nullptr;
}

// ---------------------------
// BeginPlay
// ---------------------------
void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	// --------------------
	// Создаём Core Attributes
	// --------------------
	Health = NewObject<UAttributeCore>(this);
	Stamina = NewObject<UAttributeCore>(this);
	SpiritEnergy = NewObject<UAttributeCore>(this);

	Health->Init(50.f, 100.f, 0.1f);
	Stamina->Init(50.f, 100.f, 0.1f);
	SpiritEnergy->Init(50.f, 100.f, 0.1f);

	// --------------------
	// Создаём ViewModels и привязываем к Core
	// --------------------
	HealthViewModel = NewObject<UAttributesCoreViewModel>(this);
	StaminaViewModel = NewObject<UAttributesCoreViewModel>(this);
	SpiritViewModel = NewObject<UAttributesCoreViewModel>(this);

	HealthViewModel->Init(Health);
	StaminaViewModel->Init(Stamina);
	SpiritViewModel->Init(SpiritEnergy);

	Health->OnValuesChanged.AddDynamic(this, &UAttributesComponent::HandleAttributeValuesChanged);
	Stamina->OnValuesChanged.AddDynamic(this, &UAttributesComponent::HandleAttributeValuesChanged);
	SpiritEnergy->OnValuesChanged.AddDynamic(this, &UAttributesComponent::HandleAttributeValuesChanged);
	
	// --------------------
	// Подписка на нулевой Current
	// --------------------
	Health->OnCurrentZero.AddDynamic(this, &UAttributesComponent::HandleHealthZero);
	Stamina->OnCurrentZero.AddDynamic(this, &UAttributesComponent::HandleStaminaZero);
	SpiritEnergy->OnCurrentZero.AddDynamic(this, &UAttributesComponent::HandleSpiritZero);

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		RefreshReplicatedAttributeData();
	}
	else
	{
		OnRep_HealthData();
		OnRep_StaminaData();
		OnRep_SpiritEnergyData();
	}
}

// ---------------------------
// EndPlay
// ---------------------------
void UAttributesComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Health = nullptr;
	Stamina = nullptr;
	SpiritEnergy = nullptr;
	
	HealthViewModel = nullptr;
	StaminaViewModel = nullptr;
	SpiritViewModel = nullptr;

	Super::EndPlay(EndPlayReason);
}

// ---------------------------
// TickComponent
// ---------------------------
void UAttributesComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	TimeAccumulator += DeltaTime;

	while (TimeAccumulator >= TickInterval)
	{
		if (Health)
		{
			Health->ApplyRegen();
		}
		if (Stamina)      Stamina->ApplyRegen();
		if (SpiritEnergy) SpiritEnergy->ApplyRegen();

		TimeAccumulator -= TickInterval;
	}
}

void UAttributesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAttributesComponent, HealthData);
	DOREPLIFETIME(UAttributesComponent, StaminaData);
	DOREPLIFETIME(UAttributesComponent, SpiritEnergyData);
}

// --------------------
// Callbacks: Current == 0
// --------------------
void UAttributesComponent::HandleHealthZero()
{
	OnZeroHealth.Broadcast();
}

void UAttributesComponent::HandleStaminaZero()
{
	OnZeroStamina.Broadcast();
}

void UAttributesComponent::HandleSpiritZero()
{
	OnZeroSpiritEnergy.Broadcast();
}


// --------------------
// Apply Attribute Delta
// --------------------
bool UAttributesComponent::ApplyAttributeDelta(EARPGAttributeType AttributeType, float Delta)
{
	if (GetOwner() && !GetOwner()->HasAuthority())
	{
		if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
		{
			ServerApplyAttributeDelta(AttributeType, Delta);
			return true;
		}

		return false;
	}

	return ApplyAttributeDeltaInternal(AttributeType, Delta);
}

bool UAttributesComponent::ApplyAttributeDeltaInternal(EARPGAttributeType AttributeType, float Delta)
{
	switch (AttributeType)
	{
	case EARPGAttributeType::Health:
		if (Health)
		{
			const float OldValue = Health->Current;

			Health->ChangeCurrent(Delta);

			UE_LOG(LogTemp, Warning, TEXT("ApplyAttributeDelta Health | Old=%f | Delta=%f | New=%f"),
				OldValue,
				Delta,
				Health->Current
			);

			RefreshReplicatedAttributeData();
			return true;
		}
		break;

	case EARPGAttributeType::Stamina:
		if (Stamina)
		{
			Stamina->ChangeCurrent(Delta);
			RefreshReplicatedAttributeData();
			return true;
		}
		break;

	case EARPGAttributeType::SpiritEnergy:
		if (SpiritEnergy)
		{
			SpiritEnergy->ChangeCurrent(Delta);
			RefreshReplicatedAttributeData();
			return true;
		}
		break;

	default:
		break;
	}

	return false;
}

void UAttributesComponent::ServerApplyAttributeDelta_Implementation(EARPGAttributeType AttributeType, float Delta)
{
	ApplyAttributeDeltaInternal(AttributeType, Delta);
}

FARPGAttributeReplicationData UAttributesComponent::MakeReplicationData(const UAttributeCore* Attribute) const
{
	FARPGAttributeReplicationData ReplicationData;

	if (Attribute)
	{
		ReplicationData.Current = Attribute->Current;
		ReplicationData.Max = Attribute->Max;
		ReplicationData.Buff = Attribute->Buff;
		ReplicationData.PassiveRegen = Attribute->PassiveRegen;
		ReplicationData.ActiveRegen = Attribute->ActiveRegen;
	}

	return ReplicationData;
}

void UAttributesComponent::RefreshReplicatedAttributeData()
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	HealthData = MakeReplicationData(Health);
	StaminaData = MakeReplicationData(Stamina);
	SpiritEnergyData = MakeReplicationData(SpiritEnergy);
}

void UAttributesComponent::ApplyReplicatedAttributeData(UAttributeCore* Attribute, const FARPGAttributeReplicationData& ReplicationData) const
{
	if (!Attribute)
	{
		return;
	}

	Attribute->SetValuesFromReplication(
		ReplicationData.Current,
		ReplicationData.Max,
		ReplicationData.Buff,
		ReplicationData.PassiveRegen,
		ReplicationData.ActiveRegen
	);
}

void UAttributesComponent::OnRep_HealthData()
{
	ApplyReplicatedAttributeData(Health, HealthData);
}

void UAttributesComponent::OnRep_StaminaData()
{
	ApplyReplicatedAttributeData(Stamina, StaminaData);
}

void UAttributesComponent::OnRep_SpiritEnergyData()
{
	ApplyReplicatedAttributeData(SpiritEnergy, SpiritEnergyData);
}

void UAttributesComponent::HandleAttributeValuesChanged(float Current, float Max, float Buff)
{
	RefreshReplicatedAttributeData();
}
