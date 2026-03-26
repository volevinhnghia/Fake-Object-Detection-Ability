// Copyright Epic Games, Inc. All Rights Reserved.

#include "GA_Interact.h"
#include "InteractableInterface.h"
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

	AActor* Target = FindClosestInteractable(AvatarActor);
	if (Target)
	{
		IInteractableInterface::Execute_Interact(Target, AvatarActor);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

AActor* UGA_Interact::FindClosestInteractable(AActor* AvatarActor) const
{
	if (!AvatarActor)
	{
		return nullptr;
	}

	TArray<AActor*> OverlappingActors;
	AvatarActor->GetOverlappingActors(OverlappingActors, AInteractiveBaseObject::StaticClass());

	const FVector Origin = AvatarActor->GetActorLocation();
	AActor* ClosestTarget = nullptr;
	float ClosestDistSq = MAX_FLT;

	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor->Implements<UInteractableInterface>())
		{
			continue;
		}

		if (!IInteractableInterface::Execute_CanInteract(Actor, AvatarActor))
		{
			continue;
		}

		const float DistSq = FVector::DistSquared(Origin, Actor->GetActorLocation());
		if (DistSq < ClosestDistSq)
		{
			ClosestDistSq = DistSq;
			ClosestTarget = Actor;
		}
	}

	return ClosestTarget;
}
