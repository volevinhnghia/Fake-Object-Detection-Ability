// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveBaseObject.h"
#include "InteractiveDamagePickup.generated.h"

class UGameplayEffect;

/**
 * A pickup that applies a damage GameplayEffect to the interacting player and then destroys itself.
 * Defaults to UGE_DamageHealth (-10 HP).
 */
UCLASS(Blueprintable)
class AInteractiveDamagePickup : public AInteractiveBaseObject
{
	GENERATED_BODY()

public:

	AInteractiveDamagePickup();

	virtual void Interact_Implementation(AActor* Interactor) override;

protected:

	/** The damage GameplayEffect class to apply (defaults to UGE_DamageHealth) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	TSubclassOf<UGameplayEffect> DamageEffect;
};
