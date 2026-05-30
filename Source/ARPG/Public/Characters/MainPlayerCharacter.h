#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MainPlayerCharacter.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilitySystemInitialized);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnCharacterAttributeChanged,
	float,
	CurrentValue,
	float,
	MaxValue
);

UCLASS()
class ARPG_API AMainPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMainPlayerCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintPure, Category = "GAS|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "GAS|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "GAS|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintPure, Category = "GAS|Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintPure, Category = "GAS|Attributes")
	float GetSpiritEnergy() const;

	UFUNCTION(BlueprintPure, Category = "GAS|Attributes")
	float GetMaxSpiritEnergy() const;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnCharacterAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnCharacterAttributeChanged OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnCharacterAttributeChanged OnSpiritEnergyChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS")
	FOnAbilitySystemInitialized OnAbilitySystemInitialized;
	
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintPure, Category = "GAS")
	bool IsAbilitySystemInitialized() const;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	void InitializeAbilitySystem();
	void ApplyStartupEffects();
	void ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass);

	bool bAbilitySystemInitialized = false;
	
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;

	FDelegateHandle StaminaChangedDelegateHandle;
	FDelegateHandle MaxStaminaChangedDelegateHandle;

	FDelegateHandle SpiritEnergyChangedDelegateHandle;
	FDelegateHandle MaxSpiritEnergyChangedDelegateHandle;

	void BindAttributeChangeDelegates();
	void BroadcastCurrentAttributes();

	void HandleHealthChanged(const FOnAttributeChangeData& Data);
	void HandleMaxHealthChanged(const FOnAttributeChangeData& Data);

	void HandleStaminaChanged(const FOnAttributeChangeData& Data);
	void HandleMaxStaminaChanged(const FOnAttributeChangeData& Data);

	void HandleSpiritEnergyChanged(const FOnAttributeChangeData& Data);
	void HandleMaxSpiritEnergyChanged(const FOnAttributeChangeData& Data);
};