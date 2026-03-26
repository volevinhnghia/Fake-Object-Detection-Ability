// Copyright Epic Games, Inc. All Rights Reserved.

#include "GA_Interact.h"
#include "InteractiveBaseObject.h"

UGA_Interact::UGA_Interact()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
}

void UGA_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (!AvatarActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AInteractiveBaseObject* Target = FindClosestInteractable(AvatarActor);
	if (Target)
	{
		Target->Interact(AvatarActor);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

AInteractiveBaseObject* UGA_Interact::FindClosestInteractable(AActor* AvatarActor) const
{
	if (!AvatarActor)
	{
		return nullptr;
	}

	// Query actors that overlap the player — these are actors whose trigger volumes
	// the player has entered. No traces or sweeps needed.
	TArray<AActor*> OverlappingActors;
	AvatarActor->GetOverlappingActors(OverlappingActors, AInteractiveBaseObject::StaticClass());

	const FVector Origin = AvatarActor->GetActorLocation();
	AInteractiveBaseObject* ClosestTarget = nullptr;
	float ClosestDistSq = MAX_FLT;

	for (AActor* Actor : OverlappingActors)
	{
		AInteractiveBaseObject* Interactive = Cast<AInteractiveBaseObject>(Actor);
		if (!Interactive || !Interactive->CanInteract(AvatarActor))
		{
			continue;
		}

		const float DistSq = FVector::DistSquared(Origin, Interactive->GetActorLocation());
		if (DistSq < ClosestDistSq)
		{
			ClosestDistSq = DistSq;
			ClosestTarget = Interactive;
		}
	}

	return ClosestTarget;
}
