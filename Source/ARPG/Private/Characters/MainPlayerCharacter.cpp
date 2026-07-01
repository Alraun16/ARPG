#include "Characters/MainPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/ARPGPlayerState.h"

#include "GameplayEffect.h"
#include "GAS/Attributes/CoreCharacterAttributeSet.h"
#include "GAS/Effects/CoreCharacterInitAttributesEffect.h"
#include "Characters/Components/CharacterRegenComponent.h"

AMainPlayerCharacter::AMainPlayerCharacter()
{
	bReplicates = true;
	SetReplicateMovement(true);

	PrimaryActorTick.bCanEverTick = true;

	CharacterRegenComponent = CreateDefaultSubobject<UCharacterRegenComponent>(TEXT("CharacterRegenComponent"));
}

void AMainPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAbilitySystemComponent* AMainPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMainPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilitySystem();
	ApplyStartupEffects();
}

void AMainPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAbilitySystem();
}

void AMainPlayerCharacter::InitializeAbilitySystem()
{
	if (bAbilitySystemInitialized)
	{
		return;
	}

	AARPGPlayerState* ARPGPlayerState = GetPlayerState<AARPGPlayerState>();
	if (!ARPGPlayerState)
	{
		return;
	}

	AbilitySystemComponent = ARPGPlayerState->GetAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(ARPGPlayerState, this);

	if (CharacterRegenComponent)
	{
		// MainPlayerCharacter отвечает за GAS initialization, потому что AbilitySystemComponent живёт на PlayerState.
		// RegenComponent получает уже найденный ASC и управляет только lifecycle regen effects.
		CharacterRegenComponent->Initialize(AbilitySystemComponent);
	}

	bAbilitySystemInitialized = true;

	BindAttributeChangeDelegates();
	BroadcastCurrentAttributes();

	OnAbilitySystemInitialized.Broadcast();
}

void AMainPlayerCharacter::ApplyStartupEffects()
{	
	if (!HasAuthority())
	{
		return;
	}

	AARPGPlayerState* ARPGPlayerState = GetPlayerState<AARPGPlayerState>();
	if (!ARPGPlayerState)
	{
		return;
	}

	if (ARPGPlayerState->HasAppliedStartupEffects())
	{
		return;
	}
	
	ApplyGameplayEffectToSelf(UCoreCharacterInitAttributesEffect::StaticClass());

	if (CharacterRegenComponent)
	{
		// Startup attributes применяются здесь, но lifecycle regen effects принадлежит CharacterRegenComponent.
		// Так MainPlayerCharacter не зависит от конкретных классов regen GameplayEffect.
		CharacterRegenComponent->StartDefaultRegens();
	}

	ARPGPlayerState->SetStartupEffectsApplied(true);
}

void AMainPlayerCharacter::ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass)
{
	if (!AbilitySystemComponent || !EffectClass)
	{
		return;
	}

	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		EffectClass,
		1.f,
		Context
	);

	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

float AMainPlayerCharacter::GetHealth() const
{
	return AbilitySystemComponent
		? AbilitySystemComponent->GetNumericAttribute(UCoreCharacterAttributeSet::GetHealthAttribute())
		: 0.f;
}

float AMainPlayerCharacter::GetMaxHealth() const
{
	return AbilitySystemComponent
		? AbilitySystemComponent->GetNumericAttribute(UCoreCharacterAttributeSet::GetMaxHealthAttribute())
		: 0.f;
}

float AMainPlayerCharacter::GetStamina() const
{
	return AbilitySystemComponent
		? AbilitySystemComponent->GetNumericAttribute(UCoreCharacterAttributeSet::GetStaminaAttribute())
		: 0.f;
}

float AMainPlayerCharacter::GetMaxStamina() const
{
	return AbilitySystemComponent
		? AbilitySystemComponent->GetNumericAttribute(UCoreCharacterAttributeSet::GetMaxStaminaAttribute())
		: 0.f;
}

float AMainPlayerCharacter::GetSpiritEnergy() const
{
	return AbilitySystemComponent
		? AbilitySystemComponent->GetNumericAttribute(UCoreCharacterAttributeSet::GetSpiritEnergyAttribute())
		: 0.f;
}

float AMainPlayerCharacter::GetMaxSpiritEnergy() const
{
	return AbilitySystemComponent
		? AbilitySystemComponent->GetNumericAttribute(UCoreCharacterAttributeSet::GetMaxSpiritEnergyAttribute())
		: 0.f;
}

void AMainPlayerCharacter::BindAttributeChangeDelegates()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (!HealthChangedDelegateHandle.IsValid())
	{
		HealthChangedDelegateHandle = AbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(UCoreCharacterAttributeSet::GetHealthAttribute())
			.AddUObject(this, &AMainPlayerCharacter::HandleHealthChanged);
	}

	if (!MaxHealthChangedDelegateHandle.IsValid())
	{
		MaxHealthChangedDelegateHandle = AbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(UCoreCharacterAttributeSet::GetMaxHealthAttribute())
			.AddUObject(this, &AMainPlayerCharacter::HandleMaxHealthChanged);
	}

	if (!StaminaChangedDelegateHandle.IsValid())
	{
		StaminaChangedDelegateHandle = AbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(UCoreCharacterAttributeSet::GetStaminaAttribute())
			.AddUObject(this, &AMainPlayerCharacter::HandleStaminaChanged);
	}

	if (!MaxStaminaChangedDelegateHandle.IsValid())
	{
		MaxStaminaChangedDelegateHandle = AbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(UCoreCharacterAttributeSet::GetMaxStaminaAttribute())
			.AddUObject(this, &AMainPlayerCharacter::HandleMaxStaminaChanged);
	}

	if (!SpiritEnergyChangedDelegateHandle.IsValid())
	{
		SpiritEnergyChangedDelegateHandle = AbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(UCoreCharacterAttributeSet::GetSpiritEnergyAttribute())
			.AddUObject(this, &AMainPlayerCharacter::HandleSpiritEnergyChanged);
	}

	if (!MaxSpiritEnergyChangedDelegateHandle.IsValid())
	{
		MaxSpiritEnergyChangedDelegateHandle = AbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(UCoreCharacterAttributeSet::GetMaxSpiritEnergyAttribute())
			.AddUObject(this, &AMainPlayerCharacter::HandleMaxSpiritEnergyChanged);
	}
}

void AMainPlayerCharacter::BroadcastCurrentAttributes()
{
	OnHealthChanged.Broadcast(GetHealth(), GetMaxHealth());
	OnStaminaChanged.Broadcast(GetStamina(), GetMaxStamina());
	OnSpiritEnergyChanged.Broadcast(GetSpiritEnergy(), GetMaxSpiritEnergy());
}

void AMainPlayerCharacter::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(Data.NewValue, GetMaxHealth());
}

void AMainPlayerCharacter::HandleMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(GetHealth(), Data.NewValue);
}

void AMainPlayerCharacter::HandleStaminaChanged(const FOnAttributeChangeData& Data)
{
	OnStaminaChanged.Broadcast(Data.NewValue, GetMaxStamina());
}

void AMainPlayerCharacter::HandleMaxStaminaChanged(const FOnAttributeChangeData& Data)
{
	OnStaminaChanged.Broadcast(GetStamina(), Data.NewValue);
}

void AMainPlayerCharacter::HandleSpiritEnergyChanged(const FOnAttributeChangeData& Data)
{
	OnSpiritEnergyChanged.Broadcast(Data.NewValue, GetMaxSpiritEnergy());
}

void AMainPlayerCharacter::HandleMaxSpiritEnergyChanged(const FOnAttributeChangeData& Data)
{
	OnSpiritEnergyChanged.Broadcast(GetSpiritEnergy(), Data.NewValue);
}

bool AMainPlayerCharacter::IsAbilitySystemInitialized() const
{
	return bAbilitySystemInitialized;
}