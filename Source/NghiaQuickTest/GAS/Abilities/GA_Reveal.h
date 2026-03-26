// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Reveal.generated.h"

/**
 * Toggles outline rendering on all InteractiveBaseObject actors in the world.
 * When activated, enables custom depth on every interactive object's mesh.
 * When ended, disables the outline.
 *
 * Use a post-process material with Custom Depth / Stencil to render the outline.
 */
UCLASS()
class UGA_Reveal : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_Reveal();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:

	void SetAllInteractiveOutlines(AActor* AvatarActor, bool bEnabled);
};
