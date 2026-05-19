#include "GameModes/ARPGGameMode.h"
#include "Controllers/MainPlayerController.h"
#include "Characters/MainPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AARPGGameMode::AARPGGameMode()
{
	// Указываем классы по умолчанию
	DefaultPawnClass = AMainPlayerCharacter::StaticClass();
	PlayerControllerClass = AMainPlayerController::StaticClass();
}

void AARPGGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Спавн Pawn через GameMode
	if (NewPlayer && NewPlayer->GetPawn() == nullptr)
	{
		RestartPlayer(NewPlayer); // стандартный спавн Pawn
	}

	// Здесь можно инициализировать ViewModel и HUD для нового игрока
}

