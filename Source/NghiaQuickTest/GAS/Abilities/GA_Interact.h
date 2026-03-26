// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Interact.generated.h"

/**
 * Gameplay Ability that allows the player to interact with actors
 * implementing IInteractableInterface that the player is currently overlapping.
 *
 * No traces or sweeps — purely trigger-based.
 * Picks the closest overlapping interactable that passes CanInteract().
 */
UCLASS()
class UGA_Interact : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_Interact();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	AActor* FindClosestInteractable(AActor* AvatarActor) const;
};
