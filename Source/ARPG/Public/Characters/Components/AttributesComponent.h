#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/Components/AttributeCore.h"
#include "UI/ViewModels/AttributesCoreViewModel.h"
#include "AttributesComponent.generated.h"

UENUM(BlueprintType)
enum class EARPGAttributeType : uint8
{
	Health UMETA(DisplayName = "Health"),
	Stamina UMETA(DisplayName = "Stamina"),
	SpiritEnergy UMETA(DisplayName = "Spirit Energy")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthZero);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaZero);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpiritEnergyZero);

USTRUCT()
struct FARPGAttributeReplicationData
{
	GENERATED_BODY()

	UPROPERTY()
	float Current = 50.f;

	UPROPERTY()
	float Max = 100.f;

	UPROPERTY()
	float Buff = 0.f;

	UPROPERTY()
	float PassiveRegen = 0.1f;

	UPROPERTY()
	float ActiveRegen = 0.f;
};

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
	
	// --------------------
	// Attribute Modification
	// --------------------
	UFUNCTION(BlueprintCallable, Category = "Character Attributes")
	bool ApplyAttributeDelta(EARPGAttributeType AttributeType, float Delta);
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

private:
	float TimeAccumulator;

	UPROPERTY(ReplicatedUsing=OnRep_HealthData)
	FARPGAttributeReplicationData HealthData;

	UPROPERTY(ReplicatedUsing=OnRep_StaminaData)
	FARPGAttributeReplicationData StaminaData;

	UPROPERTY(ReplicatedUsing=OnRep_SpiritEnergyData)
	FARPGAttributeReplicationData SpiritEnergyData;

	bool ApplyAttributeDeltaInternal(EARPGAttributeType AttributeType, float Delta);
	FARPGAttributeReplicationData MakeReplicationData(const UAttributeCore* Attribute) const;
	void RefreshReplicatedAttributeData();
	void ApplyReplicatedAttributeData(UAttributeCore* Attribute, const FARPGAttributeReplicationData& ReplicationData) const;

	UFUNCTION(Server, Reliable)
	void ServerApplyAttributeDelta(EARPGAttributeType AttributeType, float Delta);

	UFUNCTION()
	void OnRep_HealthData();

	UFUNCTION()
	void OnRep_StaminaData();

	UFUNCTION()
	void OnRep_SpiritEnergyData();

	UFUNCTION()
	void HandleAttributeValuesChanged(float Current, float Max, float Buff);

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
