#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttributeCore.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, float, Current, float, Max, float, Buff);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCurrentZero);

UCLASS(ClassGroup=(Attributes), BlueprintType)
class UAttributeCore : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly)
    float Current;

    UPROPERTY(BlueprintReadOnly)
    float Max;

    UPROPERTY(BlueprintReadOnly)
    float Buff;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float PassiveRegen;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ActiveRegen;

    UPROPERTY(BlueprintAssignable)
    FOnAttributeChanged OnValuesChanged;

    UPROPERTY(BlueprintAssignable)
    FOnCurrentZero OnCurrentZero;
    
    void Init(float NewCurrent, float NewMax, float NewPassive);
/*
    float GetCurrent();
    float GetMax();
    float GetDebuff();
    float GetPassiveRegenRate();
    float GetTickInterval();
*/
    void SetMax(float NewMax);
    void SetBuff(float NewDebuff);
    void SetPassiveRegen(float NewPassiveRegen);
    void SetActiveRegen(float NewActiveRegen);
    
    void ChangeCurrent(float Value);
    void ApplyRegen();
};

