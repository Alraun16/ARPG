#include "GAS/Effects/StaminaRegenEffect.h"

#include "GAS/Attributes/CoreCharacterAttributeSet.h"

UStaminaRegenEffect::UStaminaRegenEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	Period.SetValue(0.5f);
	bExecutePeriodicEffectOnApplication = false;

	FGameplayModifierInfo StaminaRegenModifier;
	StaminaRegenModifier.Attribute = UCoreCharacterAttributeSet::GetStaminaAttribute();
	StaminaRegenModifier.ModifierOp = EGameplayModOp::Additive;

	FAttributeBasedFloat StaminaRegenMagnitude;
	StaminaRegenMagnitude.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(
		UCoreCharacterAttributeSet::GetStaminaRegenPerSecondAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false
	);
	StaminaRegenMagnitude.Coefficient = FScalableFloat(0.5f);

	StaminaRegenModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(StaminaRegenMagnitude);

	Modifiers.Add(StaminaRegenModifier);
}