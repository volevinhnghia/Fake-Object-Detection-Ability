// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnCount = 5;
	bHasSpawned = false;
	bSpawnOnce = true;

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

	for (int32 i = 0; i < SpawnCount; ++i)
	{
		// Pick a random class from the array
		const int32 RandomIndex = FMath::RandRange(0, SpawnableClasses.Num() - 1);
		TSubclassOf<AActor> ClassToSpawn = SpawnableClasses[RandomIndex];
		if (!ClassToSpawn)
		{
			continue;
		}

		FVector SpawnLocation;
		if (GetRandomFloorLocation(SpawnLocation))
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			World->SpawnActor<AActor>(ClassToSpawn, SpawnLocation, FRotator::ZeroRotator, Params);
		}
	}
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
