// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveBaseObject.h"
#include "InteractiveScorePickup.generated.h"

/**
 * A pickup that destroys itself when interacted with.
 */
UCLASS(Blueprintable)
class AInteractiveScorePickup : public AInteractiveBaseObject
{
	GENERATED_BODY()

public:

	virtual void Interact_Implementation(AActor* Interactor) override;
};
