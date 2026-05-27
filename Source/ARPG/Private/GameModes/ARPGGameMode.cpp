#include "GameModes/ARPGGameMode.h"

#include "Characters/MainPlayerCharacter.h"
#include "Controllers/MainPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Player/ARPGPlayerState.h"

AARPGGameMode::AARPGGameMode()
{
	// Указываем классы по умолчанию
	DefaultPawnClass = AMainPlayerCharacter::StaticClass();
	PlayerControllerClass = AMainPlayerController::StaticClass();
	PlayerStateClass = AARPGPlayerState::StaticClass();
}

void AARPGGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!NewPlayer)
	{
		return;
	}

	static const TArray<FString> TestPlayerNames =
	{
		TEXT("Raun"),
		TEXT("Kain"),
		TEXT("Anelise")
	};

	const FString PlayerName = TestPlayerNames.IsValidIndex(NextPlayerNameIndex)
		? TestPlayerNames[NextPlayerNameIndex]
		: FString::Printf(TEXT("Player_%d"), NextPlayerNameIndex + 1);

	if (NewPlayer->PlayerState)
	{
		NewPlayer->PlayerState->SetPlayerName(PlayerName);
	}

	NextPlayerNameIndex++;

	// Спавн Pawn через GameMode
	if (NewPlayer->GetPawn() == nullptr)
	{
		RestartPlayer(NewPlayer);
	}

	// Здесь можно инициализировать ViewModel и HUD для нового игрока
}