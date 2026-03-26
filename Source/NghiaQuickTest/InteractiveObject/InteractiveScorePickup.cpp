// Copyright Epic Games, Inc. All Rights Reserved.

#include "InteractiveScorePickup.h"

void AInteractiveScorePickup::Interact(AActor* Interactor)
{
	if (!CanInteract(Interactor))
	{
		return;
	}

	Destroy();
}
