#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "TimerManager.h"
#include "CharacterRegenComponent.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EPassiveRegenResource : uint8
{
	Health,
	Stamina,
	SpiritEnergy
};

UENUM(BlueprintType)
enum class EPassiveRegenBlockReason : uint8
{
	Damage,
	ResourceSpend,
	GameplayEffect
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARPG_API UCharacterRegenComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterRegenComponent();

	void Initialize(UAbilitySystemComponent* InAbilitySystemComponent);

	void StartDefaultRegens();

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void NotifyDamageTaken();

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void NotifyStaminaSpent();

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void NotifySpiritEnergySpent();

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void BlockHealthRegenFor(float Duration, EPassiveRegenBlockReason Reason);

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void BlockStaminaRegenFor(float Duration, EPassiveRegenBlockReason Reason);

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void BlockSpiritEnergyRegenFor(float Duration, EPassiveRegenBlockReason Reason);

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void ClearHealthRegenBlockReason(EPassiveRegenBlockReason Reason);

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void ClearStaminaRegenBlockReason(EPassiveRegenBlockReason Reason);

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void ClearSpiritEnergyRegenBlockReason(EPassiveRegenBlockReason Reason);

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void ResumeHealthRegenNow();

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void ResumeStaminaRegenNow();

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void ResumeSpiritEnergyRegenNow();

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void ResumeAllRegensNow();

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void SetDamageCanBlockHealthRegen(bool bCanBlock);

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void SetDamageCanBlockStaminaRegen(bool bCanBlock);

	UFUNCTION(BlueprintCallable, Category = "GAS|Regen")
	void SetDamageCanBlockSpiritEnergyRegen(bool bCanBlock);

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

	// Timer один на ресурс, а не на причину.
	// При завершении timer компонент проверяет все active block reasons ресурса.
	FTimerHandle HealthRegenBlockTimerHandle;
	FTimerHandle StaminaRegenBlockTimerHandle;
	FTimerHandle SpiritEnergyRegenBlockTimerHandle;

	// Reasons хранят время, до которого конкретная причина блокирует passive regen.
	// Это защищает от конфликтов: damage на 5 сек не сокращает resource spend на 60 сек.
	TMap<EPassiveRegenBlockReason, double> HealthRegenBlockReasons;
	TMap<EPassiveRegenBlockReason, double> StaminaRegenBlockReasons;
	TMap<EPassiveRegenBlockReason, double> SpiritEnergyRegenBlockReasons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Regen|Delay", meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
	float RegenDelayAfterDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Regen|Delay", meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
	float StaminaRegenDelayAfterSpend = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Regen|Delay", meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
	float SpiritEnergyRegenDelayAfterSpend = 60.f;

	// Damage block включается по каждому ресурсу отдельно.
	// Например, skill может запретить damage блокировать SpiritEnergy regen,
	// но Health и Stamina regen продолжат останавливаться от damage как раньше.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Regen|Damage", meta = (AllowPrivateAccess = "true"))
	bool bDamageCanBlockHealthRegen = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Regen|Damage", meta = (AllowPrivateAccess = "true"))
	bool bDamageCanBlockStaminaRegen = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Regen|Damage", meta = (AllowPrivateAccess = "true"))
	bool bDamageCanBlockSpiritEnergyRegen = true;

	bool CanManageRegens() const;

	void BlockRegenFor(
		EPassiveRegenResource Resource,
		EPassiveRegenBlockReason Reason,
		float Duration
	);

	void ClearRegenBlockReason(
		EPassiveRegenResource Resource,
		EPassiveRegenBlockReason Reason
	);

	void ClearAllRegenBlockReasons(EPassiveRegenResource Resource);

	void ScheduleRegenRestart(EPassiveRegenResource Resource);
	void HandleRegenBlockFinished(EPassiveRegenResource Resource);

	void RemoveExpiredRegenBlockReasons(EPassiveRegenResource Resource, double CurrentTime);
	double GetLatestBlockedUntilTime(EPassiveRegenResource Resource) const;

	void StartRegen(EPassiveRegenResource Resource);
	void StopRegen(EPassiveRegenResource Resource);

	FActiveGameplayEffectHandle& GetRegenHandle(EPassiveRegenResource Resource);
	FTimerHandle& GetRegenBlockTimerHandle(EPassiveRegenResource Resource);

	TMap<EPassiveRegenBlockReason, double>& GetRegenBlockReasons(EPassiveRegenResource Resource);
	const TMap<EPassiveRegenBlockReason, double>& GetRegenBlockReasons(EPassiveRegenResource Resource) const;

	void ApplyRegenEffect(
		TSubclassOf<UGameplayEffect> EffectClass,
		FActiveGameplayEffectHandle& ActiveEffectHandle
	);

	void RemoveRegenEffect(FActiveGameplayEffectHandle& ActiveEffectHandle);
};