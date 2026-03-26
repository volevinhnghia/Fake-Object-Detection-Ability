// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

/**
 * A trigger volume that spawns random actors on the floor when a specified character enters.
 *
 * Blueprint-editable:
 *  - AllowedCharacterClass: which character class triggers the spawn
 *  - SpawnableClasses: array of actor classes to randomly pick from
 *  - SpawnCount: how many objects to spawn
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

	/** Spawn random objects on the floor inside the volume */
	void SpawnObjects();

	/** Get a random floor location inside the volume via line trace */
	bool GetRandomFloorLocation(FVector& OutLocation) const;

protected:

	// ---- Components ----

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* SpawnRegion;

	// ---- Configuration ----

	/** Character class that triggers the spawn. Leave None to allow any character. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<ACharacter> AllowedCharacterClass;

	/** Actor classes to randomly pick from when spawning */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TArray<TSubclassOf<AActor>> SpawnableClasses;

	/** Number of objects to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn", meta=(ClampMin="1"))
	int32 SpawnCount;

	/** Whether this volume has already been triggered */
	UPROPERTY(BlueprintReadOnly, Category="Spawn")
	bool bHasSpawned;

	/** If true, can only trigger once */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	bool bSpawnOnce;
};
