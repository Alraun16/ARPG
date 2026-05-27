#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ARPGPlayerState.generated.h"

class UAbilitySystemComponent;
class UCoreCharacterAttributeSet;

UCLASS()
class ARPG_API AARPGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AARPGPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UCoreCharacterAttributeSet* GetCoreCharacterAttributeSet() const;
	
	bool HasAppliedStartupEffects() const;
	void SetStartupEffectsApplied(bool bApplied);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCoreCharacterAttributeSet> CoreCharacterAttributeSet;
	
	bool bStartupEffectsApplied = false;
};