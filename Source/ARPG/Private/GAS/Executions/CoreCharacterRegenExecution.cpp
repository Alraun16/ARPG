#include "GAS/Executions/CoreCharacterRegenExecution.h"

#include "GAS/Attributes/CoreCharacterAttributeSet.h"

struct FCoreCharacterRegenStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthRegenPerSecond)
	DECLARE_ATTRIBUTE_CAPTUREDEF(StaminaRegenPerSecond)
	DECLARE_ATTRIBUTE_CAPTUREDEF(SpiritRegenPerSecond)

	FCoreCharacterRegenStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCoreCharacterAttributeSet, HealthRegenPerSecond, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCoreCharacterAttributeSet, StaminaRegenPerSecond, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCoreCharacterAttributeSet, SpiritRegenPerSecond, Target, false);
	}
};

static const FCoreCharacterRegenStatics& GetCoreCharacterRegenStatics()
{
	static FCoreCharacterRegenStatics Statics;
	return Statics;
}

UCoreCharacterRegenExecution::UCoreCharacterRegenExecution()
{
	RelevantAttributesToCapture.Add(GetCoreCharacterRegenStatics().HealthRegenPerSecondDef);
	RelevantAttributesToCapture.Add(GetCoreCharacterRegenStatics().StaminaRegenPerSecondDef);
	RelevantAttributesToCapture.Add(GetCoreCharacterRegenStatics().SpiritRegenPerSecondDef);
}

void UCoreCharacterRegenExecution::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float HealthRegenPerSecond = 0.f;
	float StaminaRegenPerSecond = 0.f;
	float SpiritRegenPerSecond = 0.f;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCoreCharacterRegenStatics().HealthRegenPerSecondDef,
		EvaluationParameters,
		HealthRegenPerSecond
	);

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCoreCharacterRegenStatics().StaminaRegenPerSecondDef,
		EvaluationParameters,
		StaminaRegenPerSecond
	);

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCoreCharacterRegenStatics().SpiritRegenPerSecondDef,
		EvaluationParameters,
		SpiritRegenPerSecond
	);

	const float RegenPeriod = 0.5f;

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			UCoreCharacterAttributeSet::GetHealthAttribute(),
			EGameplayModOp::Additive,
			HealthRegenPerSecond * RegenPeriod
		)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			UCoreCharacterAttributeSet::GetStaminaAttribute(),
			EGameplayModOp::Additive,
			StaminaRegenPerSecond * RegenPeriod
		)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			UCoreCharacterAttributeSet::GetSpiritEnergyAttribute(),
			EGameplayModOp::Additive,
			SpiritRegenPerSecond * RegenPeriod
		)
	);
}