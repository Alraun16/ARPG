#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/Components/AttributeCore.h"
#include "UI/ViewModels/AttributesCoreViewModel.h"
#include "AttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthZero);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaZero);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpiritEnergyZero);

UCLASS(ClassGroup=(Attributes), meta=(BlueprintSpawnableComponent))
class UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttributesComponent();
	
	// --------------------
	// Core Attributes
	// --------------------
	UPROPERTY(BlueprintReadOnly, Instanced, Category = "Character Attributes|Core")
	UAttributeCore* Health;

	UPROPERTY(BlueprintReadOnly, Instanced, Category = "Character Attributes|Core")
	UAttributeCore* Stamina;

	UPROPERTY(BlueprintReadOnly, Instanced, Category = "Character Attributes|Core")
	UAttributeCore* SpiritEnergy;
	
	// --------------------
	// ViewModels
	// --------------------
	UPROPERTY(BlueprintReadOnly, Category="ViewModels")
	UAttributesCoreViewModel* HealthViewModel;

	UPROPERTY(BlueprintReadOnly, Category="ViewModels")
	UAttributesCoreViewModel* StaminaViewModel;

	UPROPERTY(BlueprintReadOnly, Category="ViewModels")
	UAttributesCoreViewModel* SpiritViewModel;
	
	// --------------------
	// Delegates
	// --------------------
	UPROPERTY(BlueprintAssignable)
	FOnHealthZero OnZeroHealth;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaZero OnZeroStamina;

	UPROPERTY(BlueprintAssignable)
	FOnSpiritEnergyZero OnZeroSpiritEnergy;

	// --------------------
	// Tick settings
	// --------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Attributes|Tick")
	float TickInterval;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

private:
	float TimeAccumulator;

	// --------------------
	// Callbacks: Current == 0
	// --------------------
	UFUNCTION()
	void HandleHealthZero();

	UFUNCTION()
	void HandleStaminaZero();

	UFUNCTION()
	void HandleSpiritZero();
};
