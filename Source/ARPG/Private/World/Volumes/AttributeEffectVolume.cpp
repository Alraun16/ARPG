#include "World/Volumes/AttributeEffectVolume.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Characters/MainPlayerCharacter.h"
#include "Characters/Components/AttributesComponent.h"

AAttributeEffectVolume::AAttributeEffectVolume()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;

	EffectArea = CreateDefaultSubobject<UBoxComponent>(TEXT("EffectArea"));
	RootComponent = EffectArea;

	EffectArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EffectArea->SetCollisionObjectType(ECC_WorldDynamic);
	EffectArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	EffectArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	EffectArea->SetGenerateOverlapEvents(true);
}

void AAttributeEffectVolume::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		EffectArea->OnComponentBeginOverlap.AddDynamic(this, &AAttributeEffectVolume::HandleBeginOverlap);
		EffectArea->OnComponentEndOverlap.AddDynamic(this, &AAttributeEffectVolume::HandleEndOverlap);
	}
}

void AAttributeEffectVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bDrawDebugVolume && EffectArea)
	{
		DrawDebugBox(
			GetWorld(),
			EffectArea->GetComponentLocation(),
			EffectArea->GetScaledBoxExtent(),
			EffectArea->GetComponentQuat(),
			DebugColor,
			true,
			-1.f,
			0,
			2.f
		);
	}
	
	if (!HasAuthority())
	{
		return;
	}

	const float Delta = DeltaPerSecond * DeltaTime;

	if (OverlappingCharacters.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttributeVolume Tick | Authority=%d | Count=%d | TargetAttribute=%d | Delta=%f"),
			HasAuthority(),
			OverlappingCharacters.Num(),
			static_cast<int32>(TargetAttribute),
			Delta
		);
	}
	
	for (AMainPlayerCharacter* Character : OverlappingCharacters)
	{
		if (!Character)
		{
			continue;
		}

		UAttributesComponent* AttributesComponent = Character->FindComponentByClass<UAttributesComponent>();

		if (!AttributesComponent)
		{
			continue;
		}

		const bool bApplied = AttributesComponent->ApplyAttributeDelta(TargetAttribute, Delta);

		UE_LOG(LogTemp, Warning, TEXT("AttributeVolume ApplyAttributeDelta | Character=%s | Applied=%d"),
			*GetNameSafe(Character),
			bApplied
		);
	}
}

void AAttributeEffectVolume::HandleBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	UE_LOG(LogTemp, Warning, TEXT("AttributeVolume BeginOverlap | Authority=%d | OtherActor=%s | OtherClass=%s"),
		HasAuthority(),
		*GetNameSafe(OtherActor),
		OtherActor ? *OtherActor->GetClass()->GetName() : TEXT("None")
	);
	
	AMainPlayerCharacter* Character = Cast<AMainPlayerCharacter>(OtherActor);

	if (!Character)
	{
		return;
	}

	OverlappingCharacters.Add(Character);
	
	UE_LOG(LogTemp, Warning, TEXT("AttributeVolume added character: %s | Count=%d"),
		*GetNameSafe(Character),
		OverlappingCharacters.Num()
	);
}

void AAttributeEffectVolume::HandleEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	AMainPlayerCharacter* Character = Cast<AMainPlayerCharacter>(OtherActor);

	if (!Character)
	{
		return;
	}

	OverlappingCharacters.Remove(Character);
}