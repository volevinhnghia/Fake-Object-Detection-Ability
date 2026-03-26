// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "CountdownSubsystem.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	bHasSpawned = false;
	bSpawnOnce = true;
	CountdownTime = 30.0f;
	RemainingCorrectCount = 0;

	SpawnRegion = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnRegion"));
	RootComponent = SpawnRegion;
	SpawnRegion->SetBoxExtent(FVector(500.0f, 500.0f, 200.0f));
	SpawnRegion->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SpawnRegion->SetGenerateOverlapEvents(true);

	SpawnRegion->OnComponentBeginOverlap.AddDynamic(this, &ASpawnVolume::OnVolumeBeginOverlap);
}

void ASpawnVolume::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		return;
	}

	if (bSpawnOnce && bHasSpawned)
	{
		return;
	}

	// Check if the overlapping actor is the allowed character class
	if (AllowedCharacterClass)
	{
		if (!OtherActor->IsA(AllowedCharacterClass))
		{
			return;
		}
	}
	else
	{
		// No class specified — accept any character
		if (!OtherActor->IsA<ACharacter>())
		{
			return;
		}
	}

	SpawnObjects();
	bHasSpawned = true;

	// Start countdown via subsystem
	if (CountdownTime > 0.0f)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UCountdownSubsystem* Countdown = GI->GetSubsystem<UCountdownSubsystem>())
			{
				Countdown->StartCountdown(CountdownTime);
			}
		}
	}
}

void ASpawnVolume::SpawnObjects()
{
	if (SpawnableClasses.Num() == 0)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	RemainingCorrectCount = 0;
	SpawnedObjects.Empty();

	for (const auto& Pair : SpawnableClasses)
	{
		TSubclassOf<AInteractiveBaseObject> ClassToSpawn = Pair.Key;
		const int32 Count = Pair.Value;

		if (!ClassToSpawn || Count <= 0)
		{
			continue;
		}

		for (int32 i = 0; i < Count; ++i)
		{
			FVector SpawnLocation;
			if (GetRandomFloorLocation(SpawnLocation))
			{
				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				AActor* Spawned = World->SpawnActor<AActor>(ClassToSpawn, SpawnLocation, FRotator::ZeroRotator, Params);

				if (Spawned)
				{
					SpawnedObjects.Add(Spawned);
				}

				if (Spawned && CorrectClass && ClassToSpawn == CorrectClass)
				{
					++RemainingCorrectCount;
					Spawned->OnDestroyed.AddDynamic(this, &ASpawnVolume::OnCorrectObjectDestroyed);
				}
			}
		}
	}
}

void ASpawnVolume::OnCorrectObjectDestroyed(AActor* DestroyedActor)
{
	--RemainingCorrectCount;

	if (RemainingCorrectCount <= 0)
	{
		OnAllCorrectCollected();
	}
}

void ASpawnVolume::OnAllCorrectCollected()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UCountdownSubsystem* Countdown = GI->GetSubsystem<UCountdownSubsystem>())
		{
			Countdown->StopCountdown();
		}
	}

	for (const TWeakObjectPtr<AActor>& Obj : SpawnedObjects)
	{
		if (AActor* Actor = Obj.Get())
		{
			Actor->Destroy();
		}
	}
	SpawnedObjects.Empty();

	Destroy();
}

bool ASpawnVolume::GetRandomFloorLocation(FVector& OutLocation) const
{
	const FVector Origin = GetActorLocation();
	const FVector Extent = SpawnRegion->GetScaledBoxExtent();

	// Random XY within the box
	const float RandX = FMath::FRandRange(Origin.X - Extent.X, Origin.X + Extent.X);
	const float RandY = FMath::FRandRange(Origin.Y - Extent.Y, Origin.Y + Extent.Y);

	// Line trace down from top of box to find the floor
	const FVector TraceStart(RandX, RandY, Origin.Z + Extent.Z);
	const FVector TraceEnd(RandX, RandY, Origin.Z - Extent.Z - 500.0f);

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		OutLocation = Hit.ImpactPoint;
		return true;
	}

	// No floor found — use bottom of box
	OutLocation = FVector(RandX, RandY, Origin.Z - Extent.Z);
	return true;
}
