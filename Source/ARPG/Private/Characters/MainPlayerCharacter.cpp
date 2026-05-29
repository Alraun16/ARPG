#include "Characters/MainPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/ARPGPlayerState.h"

#include "GameplayEffect.h"
#include "GAS/Attributes/CoreCharacterAttributeSet.h"
#include "GAS/Effects/CoreCharacterInitAttributesEffect.h"
#include "GAS/Effects/HealthRegenEffect.h"
#include "GAS/Effects/StaminaRegenEffect.h"
#include "GAS/Effects/SpiritEnergyRegenEffect.h"

AMainPlayerCharacter::AMainPlayerCharacter()
{
	bReplicates = true;
	SetReplicateMovement(true);

	PrimaryActorTick.bCanEverTick = true;
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
	BindAttributeChangeDelegates();
	BroadcastCurrentAttributes();
}

void AMainPlayerCharacter::ApplyStartupEffects()
{	
	ApplyGameplayEffectToSelf(UCoreCharacterInitAttributesEffect::StaticClass());
	
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

	ApplyGameplayEffectToSelf(UHealthRegenEffect::StaticClass());
	ApplyGameplayEffectToSelf(UStaminaRegenEffect::StaticClass());
	ApplyGameplayEffectToSelf(USpiritEnergyRegenEffect::StaticClass());

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