#include "Controllers/MainPlayerController.h"

AMainPlayerController::AMainPlayerController()
{
	// Можно настроить свойства контроллера здесь
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// Инициализация при старте игры
	#if !UE_BUILD_SHIPPING
        if (IsLocalController())
        {
            ConsoleCommand(TEXT("stat fps"), true);
        }
    #endif
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// Настройка биндов ввода
}
