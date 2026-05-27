#include "Characters/MainPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/ARPGPlayerState.h"

#include "GameplayEffect.h"
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