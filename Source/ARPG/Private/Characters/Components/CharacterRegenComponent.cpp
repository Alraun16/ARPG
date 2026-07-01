#include "Characters/Components/CharacterRegenComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GAS/Effects/HealthRegenEffect.h"
#include "GAS/Effects/StaminaRegenEffect.h"
#include "GAS/Effects/SpiritEnergyRegenEffect.h"

UCharacterRegenComponent::UCharacterRegenComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterRegenComponent::Initialize(UAbilitySystemComponent* InAbilitySystemComponent)
{
	AbilitySystemComponent = InAbilitySystemComponent;
}

void UCharacterRegenComponent::StartDefaultRegens()
{
	if (!CanManageRegens())
	{
		return;
	}

	// Passive regen effects меняют replicated attributes, поэтому стартуют на сервере.
	// Клиенты получают изменения через AbilitySystemComponent replication.
	StartHealthRegen();
	StartStaminaRegen();
	StartSpiritEnergyRegen();
}

void UCharacterRegenComponent::NotifyDamageTaken()
{
	if (!CanManageRegens())
	{
		return;
	}

	// Damage — одно gameplay-событие, но реакция каждого passive regen независима.
	// Skills/buffs/debuffs смогут отключать damage block для конкретного ресурса.
	if (bDamageCanBlockHealthRegen)
	{
		BlockRegenFor(EPassiveRegenResource::Health, EPassiveRegenBlockReason::Damage, RegenDelayAfterDamage);
	}

	if (bDamageCanBlockStaminaRegen)
	{
		BlockRegenFor(EPassiveRegenResource::Stamina, EPassiveRegenBlockReason::Damage, RegenDelayAfterDamage);
	}

	if (bDamageCanBlockSpiritEnergyRegen)
	{
		BlockRegenFor(EPassiveRegenResource::SpiritEnergy, EPassiveRegenBlockReason::Damage, RegenDelayAfterDamage);
	}
}

void UCharacterRegenComponent::NotifyStaminaSpent()
{
	if (!CanManageRegens())
	{
		return;
	}

	// Трата Stamina блокирует только Stamina regen.
	// Это отдельная reason-блокировка и она не зависит от damage block.
	BlockRegenFor(
		EPassiveRegenResource::Stamina,
		EPassiveRegenBlockReason::ResourceSpend,
		StaminaRegenDelayAfterSpend
	);
}

void UCharacterRegenComponent::NotifySpiritEnergySpent()
{
	if (!CanManageRegens())
	{
		return;
	}

	// Трата SpiritEnergy блокирует только SpiritEnergy regen на длинный resource cooldown.
	BlockRegenFor(
		EPassiveRegenResource::SpiritEnergy,
		EPassiveRegenBlockReason::ResourceSpend,
		SpiritEnergyRegenDelayAfterSpend
	);
}

void UCharacterRegenComponent::BlockHealthRegenFor(float Duration, EPassiveRegenBlockReason Reason)
{
	BlockRegenFor(EPassiveRegenResource::Health, Reason, Duration);
}

void UCharacterRegenComponent::BlockStaminaRegenFor(float Duration, EPassiveRegenBlockReason Reason)
{
	BlockRegenFor(EPassiveRegenResource::Stamina, Reason, Duration);
}

void UCharacterRegenComponent::BlockSpiritEnergyRegenFor(float Duration, EPassiveRegenBlockReason Reason)
{
	BlockRegenFor(EPassiveRegenResource::SpiritEnergy, Reason, Duration);
}

void UCharacterRegenComponent::ClearHealthRegenBlockReason(EPassiveRegenBlockReason Reason)
{
	ClearRegenBlockReason(EPassiveRegenResource::Health, Reason);
}

void UCharacterRegenComponent::ClearStaminaRegenBlockReason(EPassiveRegenBlockReason Reason)
{
	ClearRegenBlockReason(EPassiveRegenResource::Stamina, Reason);
}

void UCharacterRegenComponent::ClearSpiritEnergyRegenBlockReason(EPassiveRegenBlockReason Reason)
{
	ClearRegenBlockReason(EPassiveRegenResource::SpiritEnergy, Reason);
}

void UCharacterRegenComponent::ResumeHealthRegenNow()
{
	ClearAllRegenBlockReasons(EPassiveRegenResource::Health);
}

void UCharacterRegenComponent::ResumeStaminaRegenNow()
{
	ClearAllRegenBlockReasons(EPassiveRegenResource::Stamina);
}

void UCharacterRegenComponent::ResumeSpiritEnergyRegenNow()
{
	ClearAllRegenBlockReasons(EPassiveRegenResource::SpiritEnergy);
}

void UCharacterRegenComponent::ResumeAllRegensNow()
{
	ResumeHealthRegenNow();
	ResumeStaminaRegenNow();
	ResumeSpiritEnergyRegenNow();
}

void UCharacterRegenComponent::SetDamageCanBlockHealthRegen(bool bCanBlock)
{
	if (!CanManageRegens())
	{
		return;
	}

	bDamageCanBlockHealthRegen = bCanBlock;

	if (!bDamageCanBlockHealthRegen)
	{
		// Снимаем только Damage reason.
		// Остальные причины блокировки Health regen остаются активными.
		ClearRegenBlockReason(EPassiveRegenResource::Health, EPassiveRegenBlockReason::Damage);
	}
}

void UCharacterRegenComponent::SetDamageCanBlockStaminaRegen(bool bCanBlock)
{
	if (!CanManageRegens())
	{
		return;
	}

	bDamageCanBlockStaminaRegen = bCanBlock;

	if (!bDamageCanBlockStaminaRegen)
	{
		// Damage больше не удерживает Stamina regen, но ResourceSpend или GameplayEffect всё ещё могут.
		ClearRegenBlockReason(EPassiveRegenResource::Stamina, EPassiveRegenBlockReason::Damage);
	}
}

void UCharacterRegenComponent::SetDamageCanBlockSpiritEnergyRegen(bool bCanBlock)
{
	if (!CanManageRegens())
	{
		return;
	}

	bDamageCanBlockSpiritEnergyRegen = bCanBlock;

	if (!bDamageCanBlockSpiritEnergyRegen)
	{
		// Пример для будущей "Ментальной устойчивости":
		// damage больше не удерживает SpiritEnergy regen, но трата энергии всё ещё блокирует его на 60 сек.
		ClearRegenBlockReason(EPassiveRegenResource::SpiritEnergy, EPassiveRegenBlockReason::Damage);
	}
}

void UCharacterRegenComponent::StartHealthRegen()
{
	ApplyRegenEffect(UHealthRegenEffect::StaticClass(), HealthRegenHandle);
}

void UCharacterRegenComponent::StopHealthRegen()
{
	RemoveRegenEffect(HealthRegenHandle);
}

void UCharacterRegenComponent::StartStaminaRegen()
{
	ApplyRegenEffect(UStaminaRegenEffect::StaticClass(), StaminaRegenHandle);
}

void UCharacterRegenComponent::StopStaminaRegen()
{
	RemoveRegenEffect(StaminaRegenHandle);
}

void UCharacterRegenComponent::StartSpiritEnergyRegen()
{
	ApplyRegenEffect(USpiritEnergyRegenEffect::StaticClass(), SpiritEnergyRegenHandle);
}

void UCharacterRegenComponent::StopSpiritEnergyRegen()
{
	RemoveRegenEffect(SpiritEnergyRegenHandle);
}

bool UCharacterRegenComponent::CanManageRegens() const
{
	const AActor* Owner = GetOwner();
	return Owner && Owner->HasAuthority();
}

void UCharacterRegenComponent::BlockRegenFor(
	EPassiveRegenResource Resource,
	EPassiveRegenBlockReason Reason,
	float Duration
)
{
	if (!CanManageRegens())
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (Duration <= 0.f)
	{
		ClearRegenBlockReason(Resource, Reason);
		return;
	}

	StopRegen(Resource);

	const double CurrentTime = World->GetTimeSeconds();
	const double NewBlockedUntilTime = CurrentTime + Duration;

	TMap<EPassiveRegenBlockReason, double>& BlockReasons = GetRegenBlockReasons(Resource);
	double& ExistingBlockedUntilTime = BlockReasons.FindOrAdd(Reason);

	// Одна и та же reason не должна становиться короче от нового события.
	// Например, короткий damage tick не сокращает более длинный active debuff.
	ExistingBlockedUntilTime = FMath::Max(ExistingBlockedUntilTime, NewBlockedUntilTime);

	ScheduleRegenRestart(Resource);
}

void UCharacterRegenComponent::ClearRegenBlockReason(
	EPassiveRegenResource Resource,
	EPassiveRegenBlockReason Reason
)
{
	if (!CanManageRegens())
	{
		return;
	}

	TMap<EPassiveRegenBlockReason, double>& BlockReasons = GetRegenBlockReasons(Resource);
	BlockReasons.Remove(Reason);

	ScheduleRegenRestart(Resource);
}

void UCharacterRegenComponent::ClearAllRegenBlockReasons(EPassiveRegenResource Resource)
{
	if (!CanManageRegens())
	{
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(GetRegenBlockTimerHandle(Resource));
	}

	GetRegenBlockReasons(Resource).Reset();

	// Active regen, potion или buff может вернуть passive regen раньше таймера.
	StartRegen(Resource);
}

void UCharacterRegenComponent::ScheduleRegenRestart(EPassiveRegenResource Resource)
{
	if (!CanManageRegens())
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FTimerHandle& TimerHandle = GetRegenBlockTimerHandle(Resource);
	World->GetTimerManager().ClearTimer(TimerHandle);

	const double CurrentTime = World->GetTimeSeconds();
	RemoveExpiredRegenBlockReasons(Resource, CurrentTime);

	const double LatestBlockedUntilTime = GetLatestBlockedUntilTime(Resource);
	const float RemainingTime = static_cast<float>(LatestBlockedUntilTime - CurrentTime);

	if (RemainingTime <= 0.f)
	{
		StartRegen(Resource);
		return;
	}

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &UCharacterRegenComponent::HandleRegenBlockFinished, Resource);

	World->GetTimerManager().SetTimer(
		TimerHandle,
		TimerDelegate,
		RemainingTime,
		false
	);
}

void UCharacterRegenComponent::HandleRegenBlockFinished(EPassiveRegenResource Resource)
{
	if (!CanManageRegens())
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Timer мог сработать, пока у ресурса ещё есть другая более длинная reason-блокировка.
	// Поэтому не стартуем regen напрямую, а заново пересчитываем состояние.
	RemoveExpiredRegenBlockReasons(Resource, World->GetTimeSeconds());
	ScheduleRegenRestart(Resource);
}

void UCharacterRegenComponent::RemoveExpiredRegenBlockReasons(
	EPassiveRegenResource Resource,
	double CurrentTime
)
{
	TMap<EPassiveRegenBlockReason, double>& BlockReasons = GetRegenBlockReasons(Resource);

	for (auto Iterator = BlockReasons.CreateIterator(); Iterator; ++Iterator)
	{
		if (Iterator.Value() <= CurrentTime)
		{
			Iterator.RemoveCurrent();
		}
	}
}

double UCharacterRegenComponent::GetLatestBlockedUntilTime(EPassiveRegenResource Resource) const
{
	const TMap<EPassiveRegenBlockReason, double>& BlockReasons = GetRegenBlockReasons(Resource);

	double LatestBlockedUntilTime = 0.0;

	for (const TPair<EPassiveRegenBlockReason, double>& BlockReason : BlockReasons)
	{
		LatestBlockedUntilTime = FMath::Max(LatestBlockedUntilTime, BlockReason.Value);
	}

	return LatestBlockedUntilTime;
}

void UCharacterRegenComponent::StartRegen(EPassiveRegenResource Resource)
{
	switch (Resource)
	{
	case EPassiveRegenResource::Health:
		StartHealthRegen();
		break;

	case EPassiveRegenResource::Stamina:
		StartStaminaRegen();
		break;

	case EPassiveRegenResource::SpiritEnergy:
		StartSpiritEnergyRegen();
		break;

	default:
		break;
	}
}

void UCharacterRegenComponent::StopRegen(EPassiveRegenResource Resource)
{
	switch (Resource)
	{
	case EPassiveRegenResource::Health:
		StopHealthRegen();
		break;

	case EPassiveRegenResource::Stamina:
		StopStaminaRegen();
		break;

	case EPassiveRegenResource::SpiritEnergy:
		StopSpiritEnergyRegen();
		break;

	default:
		break;
	}
}

FActiveGameplayEffectHandle& UCharacterRegenComponent::GetRegenHandle(EPassiveRegenResource Resource)
{
	switch (Resource)
	{
	case EPassiveRegenResource::Health:
		return HealthRegenHandle;

	case EPassiveRegenResource::Stamina:
		return StaminaRegenHandle;

	case EPassiveRegenResource::SpiritEnergy:
		return SpiritEnergyRegenHandle;

	default:
		return HealthRegenHandle;
	}
}

FTimerHandle& UCharacterRegenComponent::GetRegenBlockTimerHandle(EPassiveRegenResource Resource)
{
	switch (Resource)
	{
	case EPassiveRegenResource::Health:
		return HealthRegenBlockTimerHandle;

	case EPassiveRegenResource::Stamina:
		return StaminaRegenBlockTimerHandle;

	case EPassiveRegenResource::SpiritEnergy:
		return SpiritEnergyRegenBlockTimerHandle;

	default:
		return HealthRegenBlockTimerHandle;
	}
}

TMap<EPassiveRegenBlockReason, double>& UCharacterRegenComponent::GetRegenBlockReasons(
	EPassiveRegenResource Resource
)
{
	switch (Resource)
	{
	case EPassiveRegenResource::Health:
		return HealthRegenBlockReasons;

	case EPassiveRegenResource::Stamina:
		return StaminaRegenBlockReasons;

	case EPassiveRegenResource::SpiritEnergy:
		return SpiritEnergyRegenBlockReasons;

	default:
		return HealthRegenBlockReasons;
	}
}

const TMap<EPassiveRegenBlockReason, double>& UCharacterRegenComponent::GetRegenBlockReasons(
	EPassiveRegenResource Resource
) const
{
	switch (Resource)
	{
	case EPassiveRegenResource::Health:
		return HealthRegenBlockReasons;

	case EPassiveRegenResource::Stamina:
		return StaminaRegenBlockReasons;

	case EPassiveRegenResource::SpiritEnergy:
		return SpiritEnergyRegenBlockReasons;

	default:
		return HealthRegenBlockReasons;
	}
}

void UCharacterRegenComponent::ApplyRegenEffect(
	TSubclassOf<UGameplayEffect> EffectClass,
	FActiveGameplayEffectHandle& ActiveEffectHandle
)
{
	if (!CanManageRegens() || !AbilitySystemComponent || !EffectClass)
	{
		return;
	}

	if (ActiveEffectHandle.IsValid())
	{
		// Infinite regen effect остаётся активным, пока мы явно его не снимем.
		// Handle защищает от повторного применения одного и того же regen effect.
		return;
	}

	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(GetOwner());

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		EffectClass,
		1.f,
		Context
	);

	if (!SpecHandle.IsValid())
	{
		return;
	}

	// Effect применяется к AbilitySystemComponent владельца.
	// SourceObject — это metadata в effect context, а не target эффекта.
	ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UCharacterRegenComponent::RemoveRegenEffect(FActiveGameplayEffectHandle& ActiveEffectHandle)
{
	if (!CanManageRegens() || !AbilitySystemComponent || !ActiveEffectHandle.IsValid())
	{
		return;
	}

	AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEffectHandle);
	ActiveEffectHandle.Invalidate();
}