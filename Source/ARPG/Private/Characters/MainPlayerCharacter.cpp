#include "Characters/MainPlayerCharacter.h"

// ---------------------------
// Конструктор
// ---------------------------
AMainPlayerCharacter::AMainPlayerCharacter()
{	
	bReplicates = true;
	
	// Включаем тики
	PrimaryActorTick.bCanEverTick = true;

	// Создаём компонент атрибутов
	AttributesComponent = CreateDefaultSubobject<UAttributesComponent>(TEXT("AttributesComponent"));
	AttributesComponent->SetIsReplicated(true);
}

// ---------------------------
// BeginPlay
// ---------------------------
void AMainPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// ---------------------------
// Tick
// ---------------------------
void AMainPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}