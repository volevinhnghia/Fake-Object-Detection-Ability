// Copyright Epic Games, Inc. All Rights Reserved.

#include "InteractiveBaseObject.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"

AInteractiveBaseObject::AInteractiveBaseObject()
{
	PrimaryActorTick.bCanEverTick = false;

	// Defaults
	InteractionRadius = 200.0f;
	bInteractionEnabled = true;
	bShouldReveal = true;
	bRevealEnabled = false;

	// Root
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	// Mesh (visual placeholder — assign in Blueprint)
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));

	// Interaction trigger volume (overlap only, no physics blocking)
	InteractionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetSphereRadius(InteractionRadius);
	InteractionVolume->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	InteractionVolume->SetGenerateOverlapEvents(true);
	InteractionVolume->SetHiddenInGame(true);

	// Bind overlap events
	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveBaseObject::OnInteractionVolumeBeginOverlap);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AInteractiveBaseObject::OnInteractionVolumeEndOverlap);
}

// ---- Overlap Callbacks ----

void AInteractiveBaseObject::OnInteractionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !OtherActor->IsA<ACharacter>())
	{
		return;
	}

	ActorsInRange.AddUnique(OtherActor);
	OnActorEnteredRange(OtherActor);
}

void AInteractiveBaseObject::OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor)
	{
		return;
	}

	ActorsInRange.Remove(OtherActor);
	OnActorExitedRange(OtherActor);
}

// ---- Interaction API ----

bool AInteractiveBaseObject::CanInteract_Implementation(AActor* Interactor) const
{
	return bInteractionEnabled && Interactor != nullptr && IsActorInRange(Interactor);
}

bool AInteractiveBaseObject::IsActorInRange(AActor* Actor) const
{
	if (!Actor)
	{
		return false;
	}

	for (const TWeakObjectPtr<AActor>& WeakActor : ActorsInRange)
	{
		if (WeakActor.Get() == Actor)
		{
			return true;
		}
	}

	return false;
}

void AInteractiveBaseObject::Interact_Implementation(AActor* Interactor)
{
	if (!IInteractableInterface::Execute_CanInteract(this, Interactor))
	{
		return;
	}

	CurrentInteractor = Interactor;
	OnInteract(Interactor);
}

void AInteractiveBaseObject::EndInteract_Implementation(AActor* Interactor)
{
	if (CurrentInteractor.Get() == Interactor)
	{
		OnEndInteract(Interactor);
		CurrentInteractor = nullptr;
	}
}

// ---- Reveal ----

void AInteractiveBaseObject::SetRevealEnabled_Implementation(bool bEnabled)
{
	if (!bShouldReveal)
	{
		return;
	}

	bRevealEnabled = bEnabled;
	if (InteractionVolume)
	{
		InteractionVolume->SetHiddenInGame(!bEnabled);
	}
}

void AInteractiveBaseObject::ForceHideReveal_Implementation()
{
	bRevealEnabled = false;
	if (InteractionVolume)
	{
		InteractionVolume->SetHiddenInGame(true);
	}
}
