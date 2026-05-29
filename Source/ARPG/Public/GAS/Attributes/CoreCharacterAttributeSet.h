#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CoreCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class ARPG_API UCoreCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes|Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCoreCharacterAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes|Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCoreCharacterAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Attributes|Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCoreCharacterAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Attributes|Stamina")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCoreCharacterAttributeSet, MaxStamina)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SpiritEnergy, Category = "Attributes|Spirit")
	FGameplayAttributeData SpiritEnergy;
	ATTRIBUTE_ACCESSORS(UCoreCharacterAttributeSet, SpiritEnergy)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxSpiritEnergy, Category = "Attributes|Spirit")
	FGameplayAttributeData MaxSpiritEnergy;
	ATTRIBUTE_ACCESSORS(UCoreCharacterAttributeSet, MaxSpiritEnergy)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegenPerSecond, Category = "Attributes|Regen")
	FGameplayAttributeData HealthRegenPerSecond;
	ATTRIBUTE_ACCESSORS(UCoreCharacterAttributeSet, HealthRegenPerSecond)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaRegenPerSecond, Category = "Attributes|Regen")
	FGameplayAttributeData StaminaRegenPerSecond;
	ATTRIBUTE_ACCESSORS(UCoreCharacterAttributeSet, StaminaRegenPerSecond)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SpiritEnergyRegenPerSecond, Category = "Attributes|Regen")
	FGameplayAttributeData SpiritEnergyRegenPerSecond;
	ATTRIBUTE_ACCESSORS(UCoreCharacterAttributeSet, SpiritEnergyRegenPerSecond)

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_SpiritEnergy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxSpiritEnergy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_HealthRegenPerSecond(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_StaminaRegenPerSecond(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_SpiritEnergyRegenPerSecond(const FGameplayAttributeData& OldValue);
};