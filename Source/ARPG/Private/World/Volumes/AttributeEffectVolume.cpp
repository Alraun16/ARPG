#include "World/Volumes/AttributeEffectVolume.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Characters/MainPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attributes/CoreCharacterAttributeSet.h"

namespace
{
	FGameplayAttribute ResolveTargetGameplayAttribute(EAttributeEffectVolumeTarget TargetAttribute)
	{
		switch (TargetAttribute)
		{
		case EAttributeEffectVolumeTarget::Health:
			return UCoreCharacterAttributeSet::GetHealthAttribute();

		case EAttributeEffectVolumeTarget::Stamina:
			return UCoreCharacterAttributeSet::GetStaminaAttribute();

		case EAttributeEffectVolumeTarget::SpiritEnergy:
			return UCoreCharacterAttributeSet::GetSpiritEnergyAttribute();

		default:
			return FGameplayAttribute();
		}
	}
}

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

		UAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent();

		if (!AbilitySystemComponent)
		{
			continue;
		}

		const FGameplayAttribute GameplayAttribute = ResolveTargetGameplayAttribute(TargetAttribute);

		if (!GameplayAttribute.IsValid())
		{
			continue;
		}

		AbilitySystemComponent->ApplyModToAttribute(
			GameplayAttribute,
			EGameplayModOp::Additive,
			Delta
		);

		UE_LOG(LogTemp, Warning, TEXT("AttributeVolume ApplyModToAttribute | Character=%s | TargetAttribute=%d | Delta=%f"),
			*GetNameSafe(Character),
			static_cast<int32>(TargetAttribute),
			Delta
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