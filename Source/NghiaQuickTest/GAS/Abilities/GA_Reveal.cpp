// Copyright Epic Games, Inc. All Rights Reserved.

#include "GA_Reveal.h"
#include "InteractiveBaseObject.h"
#include "Kismet/GameplayStatics.h"

UGA_Reveal::UGA_Reveal()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
}

void UGA_Reveal::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* AvatarActor = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
	SetAllInteractiveOutlines(AvatarActor, true);
}

void UGA_Reveal::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AActor* AvatarActor = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
	SetAllInteractiveOutlines(AvatarActor, false);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Reveal::SetAllInteractiveOutlines(AActor* AvatarActor, bool bEnabled)
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
		if (AInteractiveBaseObject* Interactive = Cast<AInteractiveBaseObject>(Actor))
		{
			Interactive->SetOutlineEnabled(bEnabled);
		}
	}
}
