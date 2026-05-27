#include "Characters/Components/AttributeCore.h"
#include "Math/UnrealMathUtility.h"

void UAttributeCore::Init(float NewCurrent, float NewMax, float NewPassive)
{
	Current = NewCurrent;
	Max = NewMax;
	Buff = 0.f;
	PassiveRegen = NewPassive;
	ActiveRegen = 0.f;
}
/*
float UAttributeCore::GetCurrent() { return current; }
float UAttributeCore::GetMax() { return max; }
float UAttributeCore::GetDebuff() { return debuff; }
float UAttributeCore::GetPassiveRegenRate() { return passiveRegenRate; }
float UAttributeCore::GetTickInterval() { return tickInterval; };
*/
void UAttributeCore::SetMax(float NewMax)
{
	Max = NewMax;
	if (Current > Max + Buff)
		Current = Max + Buff;
	
	OnValuesChanged.Broadcast(Current, Max, Buff);
}

void UAttributeCore::SetBuff(float NewBuff)
{
	Buff = NewBuff;
	if (Current > Max + Buff)
		Current = Max + Buff;
	
	OnValuesChanged.Broadcast(Current, Max, Buff);
	if (Buff + Max == 0.f)
		OnCurrentZero.Broadcast();
}

void UAttributeCore::SetPassiveRegen(float NewPassiveRegen)
{
	PassiveRegen = NewPassiveRegen;
	OnValuesChanged.Broadcast(Current, Max, Buff);
}

void UAttributeCore::SetActiveRegen(float NewActiveRegen)
{
	ActiveRegen = NewActiveRegen;
	OnValuesChanged.Broadcast(Current, Max, Buff);
}

void UAttributeCore::ChangeCurrent(float Value)
{
	float New = FMath::Clamp(Current + Value, 0.f, Max + Buff);

	if (Current != New)
	{
		Current = New;
		OnValuesChanged.Broadcast(Current, Max, Buff);
		if (Current <= 0.f)
			OnCurrentZero.Broadcast();
	}
}

void UAttributeCore::SetValuesFromReplication(float NewCurrent, float NewMax, float NewBuff, float NewPassiveRegen, float NewActiveRegen)
{
	const bool bWasAboveZero = Current > 0.f;
	const bool bChanged =
		!FMath::IsNearlyEqual(Current, NewCurrent) ||
		!FMath::IsNearlyEqual(Max, NewMax) ||
		!FMath::IsNearlyEqual(Buff, NewBuff) ||
		!FMath::IsNearlyEqual(PassiveRegen, NewPassiveRegen) ||
		!FMath::IsNearlyEqual(ActiveRegen, NewActiveRegen);

	Current = NewCurrent;
	Max = NewMax;
	Buff = NewBuff;
	PassiveRegen = NewPassiveRegen;
	ActiveRegen = NewActiveRegen;

	if (bChanged)
	{
		OnValuesChanged.Broadcast(Current, Max, Buff);
	}

	if (bWasAboveZero && Current <= 0.f)
	{
		OnCurrentZero.Broadcast();
	}
}

void UAttributeCore::ApplyRegen()
{
	ChangeCurrent(PassiveRegen + ActiveRegen);
}



