// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ActiveGameplayEffectHandle.h"
#include "GA_Reveal.generated.h"

class UGameplayEffect;

/**
 * Shows the InteractionVolume of actors implementing IRevealableInterface
 * that have bShouldReveal = true.
 * On end, force-hides all revealable actors regardless of bShouldReveal.
 *
 * Cost setup in Blueprint:
 *  - "Cost Gameplay Effect Class": one-time cost on activate (built-in UE)
 *  - "Ongoing Cost Effect": infinite periodic GE applied while active, removed on end.
 */
UCLASS(Blueprintable)
class UGA_Reveal : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_Reveal();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	/** Infinite periodic GE that drains mana while reveal is active */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cost")
	TSubclassOf<UGameplayEffect> OngoingCostEffect;

private:

	void SetRevealOnObjects(AActor* AvatarActor, bool bEnabled);
	void ForceHideAllObjects(AActor* AvatarActor);

	void OnManaChanged(const struct FOnAttributeChangeData& Data);

	FActiveGameplayEffectHandle OngoingCostHandle;
	FDelegateHandle ManaChangedDelegateHandle;

	FGameplayAbilitySpecHandle CachedHandle;
	const FGameplayAbilityActorInfo* CachedActorInfo;
	FGameplayAbilityActivationInfo CachedActivationInfo;
};
