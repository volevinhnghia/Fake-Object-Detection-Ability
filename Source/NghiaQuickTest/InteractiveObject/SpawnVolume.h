// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveBaseObject.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

/**
 * A trigger volume that spawns random actors on the floor when a specified character enters.
 *
 * Blueprint-editable:
 *  - AllowedCharacterClass: which character class triggers the spawn
 *  - SpawnableClasses: map of object classes to spawn counts
 *  - CorrectClass: the class the player must collect to clear the volume
 *
 * When all spawned objects of CorrectClass are collected (destroyed), the volume destroys itself.
 */
UCLASS(Blueprintable)
class ASpawnVolume : public AActor
{
	GENERATED_BODY()

public:

	ASpawnVolume();

protected:

	UFUNCTION()
	void OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Spawn objects on the floor inside the volume based on SpawnableClasses map */
	void SpawnObjects();

	/** Get a random floor location inside the volume via line trace */
	bool GetRandomFloorLocation(FVector& OutLocation) const;

	/** Called when a tracked correct object is destroyed */
	UFUNCTION()
	void OnCorrectObjectDestroyed(AActor* DestroyedActor);

	/** Called when all correct objects have been collected */
	void OnAllCorrectCollected();

protected:

	// ---- Components ----

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* SpawnRegion;

	// ---- Configuration ----

	/** Character class that triggers the spawn. Leave None to allow any character. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<ACharacter> AllowedCharacterClass;

	/** Map of interactive object classes to how many of each to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TMap<TSubclassOf<AInteractiveBaseObject>, int32> SpawnableClasses;

	/** The class the player needs to collect. When all of this type are collected, the volume is cleared. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<AInteractiveBaseObject> CorrectClass;

	/** Whether this volume has already been triggered */
	UPROPERTY(BlueprintReadOnly, Category="Spawn")
	bool bHasSpawned;

	/** If true, can only trigger once */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	bool bSpawnOnce;

	// ---- Countdown ----

	/** Countdown duration in seconds. Set 0 to disable countdown. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Countdown", meta=(ClampMin="0"))
	float CountdownTime;

private:

	/** Number of correct objects still alive */
	int32 RemainingCorrectCount;

	/** All spawned objects tracked for cleanup */
	TArray<TWeakObjectPtr<AActor>> SpawnedObjects;
};
