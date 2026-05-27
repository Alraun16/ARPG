#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/Components/AttributesComponent.h"
#include "AttributeEffectVolume.generated.h"

class UBoxComponent;
class AMainPlayerCharacter;

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
	EARPGAttributeType TargetAttribute = EARPGAttributeType::Health;

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