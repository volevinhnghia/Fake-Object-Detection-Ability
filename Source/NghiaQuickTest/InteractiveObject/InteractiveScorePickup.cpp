// Copyright Epic Games, Inc. All Rights Reserved.

#include "InteractiveScorePickup.h"

void AInteractiveScorePickup::Interact_Implementation(AActor* Interactor)
{
	if (!IInteractableInterface::Execute_CanInteract(this, Interactor))
	{
		return;
	}

	Destroy();
}
