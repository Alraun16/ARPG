#include "Player/ARPGPlayerState.h"

#include "AbilitySystemComponent.h"
#include "GAS/Attributes/CoreCharacterAttributeSet.h"

AARPGPlayerState::AARPGPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	CoreCharacterAttributeSet = CreateDefaultSubobject<UCoreCharacterAttributeSet>(TEXT("CoreCharacterAttributeSet"));

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AARPGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCoreCharacterAttributeSet* AARPGPlayerState::GetCoreCharacterAttributeSet() const
{
	return CoreCharacterAttributeSet;
}

bool AARPGPlayerState::HasAppliedStartupEffects() const
{
	return bStartupEffectsApplied;
}

void AARPGPlayerState::SetStartupEffectsApplied(bool bApplied)
{
	bStartupEffectsApplied = bApplied;
}