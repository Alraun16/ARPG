#include "Controllers/MainPlayerController.h"

#include "AbilitySystemComponent.h"
#include "Characters/MainPlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "EngineUtils.h"
#include "GAS/Attributes/CoreCharacterAttributeSet.h"
#include "GameFramework/PlayerState.h"

AMainPlayerController::AMainPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

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

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if !UE_BUILD_SHIPPING
	DrawDebugCharacterAttributes();
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

void AMainPlayerController::DrawDebugCharacterAttributes() const
{
	if (!IsLocalController())
	{
		return;
	}

	if (!GEngine || !GetWorld())
	{
		return;
	}

	TArray<AMainPlayerCharacter*> PlayerCharacters;

	for (TActorIterator<AMainPlayerCharacter> It(GetWorld()); It; ++It)
	{
		if (AMainPlayerCharacter* PlayerCharacter = *It)
		{
			PlayerCharacters.Add(PlayerCharacter);
		}
	}

	PlayerCharacters.Sort([](const AMainPlayerCharacter& Left, const AMainPlayerCharacter& Right)
	{
		const APlayerState* LeftPlayerState = Left.GetPlayerState();
		const APlayerState* RightPlayerState = Right.GetPlayerState();

		const int32 LeftId = LeftPlayerState ? LeftPlayerState->GetPlayerId() : INDEX_NONE;
		const int32 RightId = RightPlayerState ? RightPlayerState->GetPlayerId() : INDEX_NONE;

		return LeftId < RightId;
	});

	constexpr int32 BaseMessageKey = 10000;
	constexpr float MessageDuration = 0.f;

	GEngine->AddOnScreenDebugMessage(
		BaseMessageKey,
		MessageDuration,
		FColor::Cyan,
		FString::Printf(TEXT("Characters: %d"), PlayerCharacters.Num())
	);

	for (int32 Index = 0; Index < PlayerCharacters.Num(); ++Index)
	{
		const AMainPlayerCharacter* PlayerCharacter = PlayerCharacters[Index];
		if (!PlayerCharacter)
		{
			continue;
		}

		const UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();

		const float Health = ASC
			? ASC->GetNumericAttribute(UCoreCharacterAttributeSet::GetHealthAttribute())
			: -1.f;

		const float MaxHealth = ASC
			? ASC->GetNumericAttribute(UCoreCharacterAttributeSet::GetMaxHealthAttribute())
			: -1.f;

		const APlayerState* CharacterPlayerState = PlayerCharacter->GetPlayerState();

		const FString PlayerName = CharacterPlayerState
			? CharacterPlayerState->GetPlayerName()
			: TEXT("NoPlayerState");

		const FString Message = FString::Printf(
			TEXT("[%d] %s | HP: %.1f / %.1f | ASC: %s"),
			Index,
			*PlayerName,
			Health,
			MaxHealth,
			ASC ? TEXT("valid") : TEXT("null")
		);

		GEngine->AddOnScreenDebugMessage(
			BaseMessageKey + 1 + Index,
			MessageDuration,
			FColor::Yellow,
			Message
		);
	}
}