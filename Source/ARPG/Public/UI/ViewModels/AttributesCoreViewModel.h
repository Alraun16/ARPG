#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Characters/Components/AttributeCore.h"
#include "AttributesCoreViewModel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnViewModelChanged, float, Current, float, Max, float, Buff);

UCLASS(BlueprintType)
class UAttributesCoreViewModel : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	float Current;

	UPROPERTY(BlueprintReadOnly)
	float Max;

	UPROPERTY(BlueprintReadOnly)
	float Buff;

	UPROPERTY(BlueprintAssignable)
	FOnViewModelChanged OnViewModelChanged;

	void Init(UAttributeCore* Attribute);

private:
	UPROPERTY()
	UAttributeCore* CoreAttribute;

	UFUNCTION()
	void UpdateValues(float NewCurrent, float NewMax, float NewBuff);
};
