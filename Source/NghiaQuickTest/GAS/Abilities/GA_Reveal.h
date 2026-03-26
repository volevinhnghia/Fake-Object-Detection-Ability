// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Reveal.generated.h"

/**
 * Shows the InteractionVolume of actors implementing IRevealableInterface
 * that have bShouldReveal = true.
 * On end, force-hides all revealable actors regardless of bShouldReveal.
 */
UCLASS(Blueprintable)
class UGA_Reveal : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_Reveal();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:

	void SetRevealOnObjects(AActor* AvatarActor, bool bEnabled);
	void ForceHideAllObjects(AActor* AvatarActor);
};
