#include "Controllers/MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

AMainPlayerController::AMainPlayerController()
{
	DefaultMappingContext = nullptr;
	DefaultMappingPriority = 0;
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AddDefaultMappingContext();
	
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

void AMainPlayerController::AddDefaultMappingContext()
{
	if (!IsLocalController())
	{
		return;
	}

	if (!DefaultMappingContext)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSubsystem)
	{
		return;
	}

	InputSubsystem->AddMappingContext(DefaultMappingContext, DefaultMappingPriority);
}
