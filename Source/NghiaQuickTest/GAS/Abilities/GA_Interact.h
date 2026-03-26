// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Interact.generated.h"

class AInteractiveBaseObject;

/**
 * Gameplay Ability that allows the player to interact with InteractiveBaseObjects
 * that the player is currently overlapping (inside their InteractionVolume trigger).
 *
 * No traces or sweeps — purely trigger-based.
 * Picks the closest overlapping InteractiveBaseObject that passes CanInteract().
 *
 * Add this ability to the character's DefaultAbilities array, then activate via
 * AbilitySystemComponent->TryActivateAbilityByClass(UGA_Interact::StaticClass()).
 */
UCLASS()
class UGA_Interact : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_Interact();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	/** Finds the closest InteractiveBaseObject that the player is currently overlapping */
	AInteractiveBaseObject* FindClosestInteractable(AActor* AvatarActor) const;
};
