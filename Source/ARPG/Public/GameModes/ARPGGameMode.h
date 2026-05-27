#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARPGGameMode.generated.h"

UCLASS()
class ARPG_API AARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AARPGGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	int32 NextPlayerNameIndex = 0;
};