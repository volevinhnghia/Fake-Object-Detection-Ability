// Copyright Epic Games, Inc. All Rights Reserved.

#include "CountdownSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UCountdownSubsystem::StartCountdown(float Duration)
{
	if (Duration <= 0.0f)
	{
		return;
	}

	RemainingTime = Duration;
	bIsActive = true;

	// Broadcast initial time
	OnCountdownTick.Broadcast(RemainingTime);

	// Tick every 1 second
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(
			CountdownTimerHandle,
			this,
			&UCountdownSubsystem::Tick,
			1.0f,
			true
		);
	}
}

void UCountdownSubsystem::StopCountdown()
{
	bIsActive = false;

	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(CountdownTimerHandle);
	}
}

void UCountdownSubsystem::Tick()
{
	RemainingTime -= 1.0f;

	if (RemainingTime <= 0.0f)
	{
		RemainingTime = 0.0f;
		StopCountdown();
		OnCountdownTick.Broadcast(0.0f);
		OnCountdownFinished.Broadcast();
	}
	else
	{
		OnCountdownTick.Broadcast(RemainingTime);
	}
}
