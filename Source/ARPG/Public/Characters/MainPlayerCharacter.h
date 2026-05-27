#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MainPlayerCharacter.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UCLASS()
class ARPG_API AMainPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMainPlayerCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	void InitializeAbilitySystem();
	void ApplyStartupEffects();
	void ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass);
};