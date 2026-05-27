#include "GAS/Effects/HealthRegenEffect.h"

#include "GAS/Attributes/CoreCharacterAttributeSet.h"

UHealthRegenEffect::UHealthRegenEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	Period.SetValue(0.5f);
	bExecutePeriodicEffectOnApplication = false;

	FGameplayModifierInfo HealthRegenModifier;
	HealthRegenModifier.Attribute = UCoreCharacterAttributeSet::GetHealthAttribute();
	HealthRegenModifier.ModifierOp = EGameplayModOp::Additive;

	FAttributeBasedFloat HealthRegenMagnitude;
	HealthRegenMagnitude.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(
		UCoreCharacterAttributeSet::GetHealthRegenPerSecondAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false
	);
	HealthRegenMagnitude.Coefficient = FScalableFloat(0.5f);

	HealthRegenModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(HealthRegenMagnitude);

	Modifiers.Add(HealthRegenModifier);
}