#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "CoreCharacterRegenExecution.generated.h"

UCLASS()
class ARPG_API UCoreCharacterRegenExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UCoreCharacterRegenExecution();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput
	) const override;
};