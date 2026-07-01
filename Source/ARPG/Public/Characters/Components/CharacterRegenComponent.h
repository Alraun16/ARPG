#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "CharacterRegenComponent.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARPG_API UCharacterRegenComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterRegenComponent();

	void Initialize(UAbilitySystemComponent* InAbilitySystemComponent);

	void StartDefaultRegens();

	void StartHealthRegen();
	void StopHealthRegen();

	void StartStaminaRegen();
	void StopStaminaRegen();

	void StartSpiritEnergyRegen();
	void StopSpiritEnergyRegen();

private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// Infinite GameplayEffect нужно снимать по active effect handle.
	// Отдельный handle на каждый ресурс позволяет независимо выключать Health/Stamina/SpiritEnergy regen.
	FActiveGameplayEffectHandle HealthRegenHandle;
	FActiveGameplayEffectHandle StaminaRegenHandle;
	FActiveGameplayEffectHandle SpiritEnergyRegenHandle;

	void ApplyRegenEffect(
		TSubclassOf<UGameplayEffect> EffectClass,
		FActiveGameplayEffectHandle& ActiveEffectHandle
	);

	void RemoveRegenEffect(FActiveGameplayEffectHandle& ActiveEffectHandle);
};