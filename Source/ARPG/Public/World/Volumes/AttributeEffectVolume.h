#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttributeEffectVolume.generated.h"

class UBoxComponent;
class AMainPlayerCharacter;

UENUM(BlueprintType)
enum class EAttributeEffectVolumeTarget : uint8
{
	Health,
	Stamina,
	SpiritEnergy
};

UCLASS()
class ARPG_API AAttributeEffectVolume : public AActor
{
	GENERATED_BODY()

public:
	AAttributeEffectVolume();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* EffectArea;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute Effect|Debug")
	bool bDrawDebugVolume = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute Effect|Debug")
	FColor DebugColor = FColor::White;
	
	// --------------------
	// Attribute Effect Settings
	// --------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute Effect")
	EAttributeEffectVolumeTarget TargetAttribute = EAttributeEffectVolumeTarget::Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute Effect")
	float DeltaPerSecond = -10.f;

private:
	UPROPERTY()
	TSet<AMainPlayerCharacter*> OverlappingCharacters;
	
	UFUNCTION()
	void HandleBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void HandleEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
};