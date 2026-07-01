#include "Characters/Components/CharacterRegenComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GAS/Effects/HealthRegenEffect.h"
#include "GAS/Effects/StaminaRegenEffect.h"
#include "GAS/Effects/SpiritEnergyRegenEffect.h"

UCharacterRegenComponent::UCharacterRegenComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterRegenComponent::Initialize(UAbilitySystemComponent* InAbilitySystemComponent)
{
	AbilitySystemComponent = InAbilitySystemComponent;
}

void UCharacterRegenComponent::StartDefaultRegens()
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	// GameplayEffect, который меняет replicated attributes, применяем только на сервере.
	// Клиенты получают итоговые изменения attributes через replication AbilitySystemComponent.
	StartHealthRegen();
	StartStaminaRegen();
	StartSpiritEnergyRegen();
}

void UCharacterRegenComponent::StartHealthRegen()
{
	ApplyRegenEffect(UHealthRegenEffect::StaticClass(), HealthRegenHandle);
}

void UCharacterRegenComponent::StopHealthRegen()
{
	RemoveRegenEffect(HealthRegenHandle);
}

void UCharacterRegenComponent::StartStaminaRegen()
{
	ApplyRegenEffect(UStaminaRegenEffect::StaticClass(), StaminaRegenHandle);
}

void UCharacterRegenComponent::StopStaminaRegen()
{
	RemoveRegenEffect(StaminaRegenHandle);
}

void UCharacterRegenComponent::StartSpiritEnergyRegen()
{
	ApplyRegenEffect(USpiritEnergyRegenEffect::StaticClass(), SpiritEnergyRegenHandle);
}

void UCharacterRegenComponent::StopSpiritEnergyRegen()
{
	RemoveRegenEffect(SpiritEnergyRegenHandle);
}

void UCharacterRegenComponent::ApplyRegenEffect(
	TSubclassOf<UGameplayEffect> EffectClass,
	FActiveGameplayEffectHandle& ActiveEffectHandle
)
{
	if (!AbilitySystemComponent || !EffectClass)
	{
		return;
	}

	if (ActiveEffectHandle.IsValid())
	{
		// Infinite regen effect остаётся активным, пока мы явно его не снимем.
		// Handle защищает от повторного применения одного и того же regen effect.
		return;
	}

	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(GetOwner());

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		EffectClass,
		1.f,
		Context
	);

	if (!SpecHandle.IsValid())
	{
		return;
	}

	// Effect применяется к AbilitySystemComponent владельца.
	// SourceObject — это metadata в effect context, а не target эффекта.
	ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UCharacterRegenComponent::RemoveRegenEffect(FActiveGameplayEffectHandle& ActiveEffectHandle)
{
	if (!AbilitySystemComponent || !ActiveEffectHandle.IsValid())
	{
		return;
	}

	AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEffectHandle);
	ActiveEffectHandle.Invalidate();
}