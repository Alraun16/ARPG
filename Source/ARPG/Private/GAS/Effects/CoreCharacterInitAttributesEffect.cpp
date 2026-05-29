#include "GAS/Effects/CoreCharacterInitAttributesEffect.h"

#include "GAS/Attributes/CoreCharacterAttributeSet.h"

namespace
{
	void AddOverrideModifier(
		TArray<FGameplayModifierInfo>& Modifiers,
		const FGameplayAttribute& Attribute,
		float Value
	)
	{
		FGameplayModifierInfo Modifier;
		Modifier.Attribute = Attribute;
		Modifier.ModifierOp = EGameplayModOp::Override;
		Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(Value));

		Modifiers.Add(Modifier);
	}
}

UCoreCharacterInitAttributesEffect::UCoreCharacterInitAttributesEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	AddOverrideModifier(Modifiers, UCoreCharacterAttributeSet::GetMaxHealthAttribute(), 100.f);
	AddOverrideModifier(Modifiers, UCoreCharacterAttributeSet::GetHealthAttribute(), 50.f);

	AddOverrideModifier(Modifiers, UCoreCharacterAttributeSet::GetMaxStaminaAttribute(), 100.f);
	AddOverrideModifier(Modifiers, UCoreCharacterAttributeSet::GetStaminaAttribute(), 50.f);

	AddOverrideModifier(Modifiers, UCoreCharacterAttributeSet::GetMaxSpiritEnergyAttribute(), 100.f);
	AddOverrideModifier(Modifiers, UCoreCharacterAttributeSet::GetSpiritEnergyAttribute(), 50.f);

	AddOverrideModifier(Modifiers, UCoreCharacterAttributeSet::GetHealthRegenPerSecondAttribute(), 1);
	AddOverrideModifier(Modifiers, UCoreCharacterAttributeSet::GetStaminaRegenPerSecondAttribute(), 1);
	AddOverrideModifier(Modifiers, UCoreCharacterAttributeSet::GetSpiritEnergyRegenPerSecondAttribute(), 1);
}