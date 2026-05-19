#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/Components/AttributesComponent.h"
#include "MainPlayerCharacter.generated.h"


UCLASS()
class ARPG_API AMainPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Конструктор
	AMainPlayerCharacter();

	/** Компонент атрибутов (Health, Stamina, Spirit) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	UAttributesComponent* AttributesComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
