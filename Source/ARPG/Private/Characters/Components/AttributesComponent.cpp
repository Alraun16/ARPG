#include "Characters/Components/AttributesComponent.h"
#include "Characters/Components/AttributeCore.h"


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
	
	// --------------------
	// Подписка на нулевой Current
	// --------------------
	Health->OnCurrentZero.AddDynamic(this, &UAttributesComponent::HandleHealthZero);
	Stamina->OnCurrentZero.AddDynamic(this, &UAttributesComponent::HandleStaminaZero);
	SpiritEnergy->OnCurrentZero.AddDynamic(this, &UAttributesComponent::HandleSpiritZero);
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

	TimeAccumulator += DeltaTime;

	while (TimeAccumulator >= TickInterval)
	{
		if (Health)       Health->ApplyRegen();
		if (Stamina)      Stamina->ApplyRegen();
		if (SpiritEnergy) SpiritEnergy->ApplyRegen();

		TimeAccumulator -= TickInterval;
	}
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
