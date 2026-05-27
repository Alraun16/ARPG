#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "HealthRegenEffect.generated.h"

UCLASS()
class ARPG_API UHealthRegenEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UHealthRegenEffect();
};