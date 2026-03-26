// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "RevealableInterface.h"
#include "InteractiveBaseObject.generated.h"

class USphereComponent;
class UStaticMeshComponent;

/**
 * Base class for objects that can be interacted with via the Gameplay Ability System.
 *
 * The InteractionVolume sphere generates overlap events when the player enters/exits.
 * GA_Interact queries the player's overlapping actors to find valid targets — no traces needed.
 *
 * Override OnInteract / OnEndInteract in Blueprint for custom behavior.
 */
UCLASS(Blueprintable)
class AInteractiveBaseObject : public AActor, public IRevealableInterface
{
	GENERATED_BODY()

public:

	AInteractiveBaseObject();

	// ---- Interaction API (called by GA_Interact) ----

	/** Called when a player begins interacting with this object */
	UFUNCTION(BlueprintCallable, Category="Interaction")
	virtual void Interact(AActor* Interactor);

	/** Called when a player ends interaction with this object */
	UFUNCTION(BlueprintCallable, Category="Interaction")
	virtual void EndInteract(AActor* Interactor);

	/** Returns true if this object can currently be interacted with */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	bool CanInteract(AActor* Interactor) const;

	/** Returns true if the given actor is currently inside the InteractionVolume */
	UFUNCTION(BlueprintCallable, Category="Interaction")
	bool IsActorInRange(AActor* Actor) const;

	/** Returns the interaction detection sphere */
	UFUNCTION(BlueprintCallable, Category="Interaction")
	USphereComponent* GetInteractionVolume() const { return InteractionVolume; }

protected:

	// ---- Blueprint Events ----

	/** Blueprint event fired when interaction begins */
	UFUNCTION(BlueprintImplementableEvent, Category="Interaction")
	void OnInteract(AActor* Interactor);

	/** Blueprint event fired when interaction ends */
	UFUNCTION(BlueprintImplementableEvent, Category="Interaction")
	void OnEndInteract(AActor* Interactor);

	/** Blueprint event fired when an actor enters the interaction range */
	UFUNCTION(BlueprintImplementableEvent, Category="Interaction")
	void OnActorEnteredRange(AActor* Actor);

	/** Blueprint event fired when an actor exits the interaction range */
	UFUNCTION(BlueprintImplementableEvent, Category="Interaction")
	void OnActorExitedRange(AActor* Actor);

protected:

	// ---- Overlap Callbacks ----

	UFUNCTION()
	void OnInteractionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:

	// ---- Components ----

	/** Root scene component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* SceneRoot;

	/** Optional static mesh for visual representation (set in Blueprint) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* MeshComponent;

	/** Sphere trigger used to detect when the player is in interaction range */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USphereComponent* InteractionVolume;

	// ---- Configuration ----

	/** Radius within which the player can interact with this object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction", meta=(ClampMin="10.0", ClampMax="1000.0"))
	float InteractionRadius;

	/** Gameplay tag that identifies this object's interaction type (e.g. Interaction.Pickup, Interaction.Door) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	FGameplayTag InteractionTag;

	/** Whether this object is currently enabled for interaction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool bInteractionEnabled;

	/** Whether GA_Reveal should affect this object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool bShouldReveal;

	/** The actor currently interacting with this object (nullptr if none) */
	UPROPERTY(BlueprintReadOnly, Category="Interaction")
	TWeakObjectPtr<AActor> CurrentInteractor;

	/** Actors currently inside the InteractionVolume trigger */
	TArray<TWeakObjectPtr<AActor>> ActorsInRange;

public:

	// ---- Reveal API (IRevealableInterface) ----

	virtual void SetRevealEnabled_Implementation(bool bEnabled) override;
	virtual void ForceHideReveal_Implementation() override;

	/** Returns true if the reveal visualization is currently active */
	UFUNCTION(BlueprintCallable, Category="Interaction")
	bool IsRevealEnabled() const { return bRevealEnabled; }

protected:

	/** Whether the reveal visualization is currently active */
	UPROPERTY(BlueprintReadOnly, Category="Interaction")
	bool bRevealEnabled;
};
