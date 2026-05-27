#include "GAS/Effects/SpiritEnergyRegenEffect.h"

#include "GAS/Attributes/CoreCharacterAttributeSet.h"

USpiritEnergyRegenEffect::USpiritEnergyRegenEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	Period.SetValue(0.5f);
	bExecutePeriodicEffectOnApplication = false;

	FGameplayModifierInfo SpiritRegenModifier;
	SpiritRegenModifier.Attribute = UCoreCharacterAttributeSet::GetSpiritEnergyAttribute();
	SpiritRegenModifier.ModifierOp = EGameplayModOp::Additive;

	FAttributeBasedFloat SpiritRegenMagnitude;
	SpiritRegenMagnitude.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(
		UCoreCharacterAttributeSet::GetSpiritEnergyRegenPerSecondAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false
	);
	SpiritRegenMagnitude.Coefficient = FScalableFloat(0.5f);

	SpiritRegenModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(SpiritRegenMagnitude);

	Modifiers.Add(SpiritRegenModifier);
}