#include "Player/ARPGPlayerState.h"

#include "AbilitySystemComponent.h"
#include "GAS/Attributes/CoreCharacterAttributeSet.h"

AARPGPlayerState::AARPGPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UCoreCharacterAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AARPGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCoreCharacterAttributeSet* AARPGPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}