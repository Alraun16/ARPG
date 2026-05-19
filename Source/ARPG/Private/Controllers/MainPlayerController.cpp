#include "Controllers/MainPlayerController.h"

AMainPlayerController::AMainPlayerController()
{
	// Можно настроить свойства контроллера здесь
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// Инициализация при старте игры
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// Настройка биндов ввода
}
