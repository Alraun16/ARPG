#include "GAS/Attributes/CoreCharacterAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UCoreCharacterAttributeSet::UCoreCharacterAttributeSet()
{
	InitMaxHealth(100.f);
	InitHealth(50.f);

	InitMaxStamina(100.f);
	InitStamina(50.f);

	InitMaxSpiritEnergy(100.f);
	InitSpiritEnergy(50.f);

	InitHealthRegenPerSecond(0.5f);
	InitStaminaRegenPerSecond(0.5f);
	InitSpiritEnergyRegenPerSecond(0.5f);
}

void UCoreCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCoreCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCoreCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UCoreCharacterAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCoreCharacterAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UCoreCharacterAttributeSet, SpiritEnergy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCoreCharacterAttributeSet, MaxSpiritEnergy, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UCoreCharacterAttributeSet, HealthRegenPerSecond, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCoreCharacterAttributeSet, StaminaRegenPerSecond, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCoreCharacterAttributeSet, SpiritEnergyRegenPerSecond, COND_None, REPNOTIFY_Always);
}

void UCoreCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
	else if (Attribute == GetSpiritEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxSpiritEnergy());
	}
}

void UCoreCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

		if (GetHealth() <= 0.f)
		{
			// Позже сюда добавим смерть / State.Dead.
		}
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
	else if (Data.EvaluatedData.Attribute == GetSpiritEnergyAttribute())
	{
		SetSpiritEnergy(FMath::Clamp(GetSpiritEnergy(), 0.f, GetMaxSpiritEnergy()));
	}
}

void UCoreCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreCharacterAttributeSet, Health, OldValue);
}

void UCoreCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreCharacterAttributeSet, MaxHealth, OldValue);
}

void UCoreCharacterAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreCharacterAttributeSet, Stamina, OldValue);
}

void UCoreCharacterAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreCharacterAttributeSet, MaxStamina, OldValue);
}

void UCoreCharacterAttributeSet::OnRep_SpiritEnergy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreCharacterAttributeSet, SpiritEnergy, OldValue);
}

void UCoreCharacterAttributeSet::OnRep_MaxSpiritEnergy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreCharacterAttributeSet, MaxSpiritEnergy, OldValue);
}

void UCoreCharacterAttributeSet::OnRep_HealthRegenPerSecond(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreCharacterAttributeSet, HealthRegenPerSecond, OldValue);
}

void UCoreCharacterAttributeSet::OnRep_StaminaRegenPerSecond(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreCharacterAttributeSet, StaminaRegenPerSecond, OldValue);
}

void UCoreCharacterAttributeSet::OnRep_SpiritEnergyRegenPerSecond(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreCharacterAttributeSet, SpiritEnergyRegenPerSecond, OldValue);
}