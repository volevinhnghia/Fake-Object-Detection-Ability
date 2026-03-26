// Copyright Epic Games, Inc. All Rights Reserved.

#include "GA_Reveal.h"
#include "RevealableInterface.h"
#include "InteractiveBaseObject.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.h"
#include "Kismet/GameplayStatics.h"

UGA_Reveal::UGA_Reveal()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
	CachedActorInfo = nullptr;
}

void UGA_Reveal::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CachedHandle = Handle;
	CachedActorInfo = ActorInfo;
	CachedActivationInfo = ActivationInfo;

	UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;

	// Apply ongoing periodic cost
	if (ASC && OngoingCostEffect)
	{
		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		Context.AddSourceObject(ActorInfo->AvatarActor.Get());
		OngoingCostHandle = ASC->ApplyGameplayEffectToSelf(
			OngoingCostEffect.GetDefaultObject(), 1, Context);

		// Listen for mana reaching zero to auto-end
		ManaChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(
			UBaseAttributeSet::GetManaAttribute()).AddUObject(this, &UGA_Reveal::OnManaChanged);
	}

	AActor* AvatarActor = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
	SetRevealOnObjects(AvatarActor, true);
}

void UGA_Reveal::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;

	// Remove ongoing cost and stop listening
	if (ASC)
	{
		if (OngoingCostHandle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(OngoingCostHandle);
			OngoingCostHandle.Invalidate();
		}

		ASC->GetGameplayAttributeValueChangeDelegate(
			UBaseAttributeSet::GetManaAttribute()).Remove(ManaChangedDelegateHandle);
	}

	AActor* AvatarActor = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
	ForceHideAllObjects(AvatarActor);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Reveal::OnManaChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0.0f)
	{
		EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, true, true);
	}
}

void UGA_Reveal::SetRevealOnObjects(AActor* AvatarActor, bool bEnabled)
{
	if (!AvatarActor)
	{
		return;
	}

	UWorld* World = AvatarActor->GetWorld();
	if (!World)
	{
		return;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, AInteractiveBaseObject::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		IRevealableInterface::Execute_SetRevealEnabled(Actor, bEnabled);
	}
}

void UGA_Reveal::ForceHideAllObjects(AActor* AvatarActor)
{
	if (!AvatarActor)
	{
		return;
	}

	UWorld* World = AvatarActor->GetWorld();
	if (!World)
	{
		return;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, AInteractiveBaseObject::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		IRevealableInterface::Execute_ForceHideReveal(Actor);
	}
}
