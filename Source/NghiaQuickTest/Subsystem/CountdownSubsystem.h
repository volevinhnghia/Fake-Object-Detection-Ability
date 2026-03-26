// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CountdownSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownTick, float, RemainingTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCountdownFinished);

/**
 * Subsystem that manages a countdown timer.
 * SpawnVolume starts it, Widget displays it, player dies when it reaches zero.
 */
UCLASS()
class UCountdownSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	/** Start a countdown with the given duration in seconds */
	UFUNCTION(BlueprintCallable, Category="Countdown")
	void StartCountdown(float Duration);

	/** Stop the countdown */
	UFUNCTION(BlueprintCallable, Category="Countdown")
	void StopCountdown();

	/** Returns true if a countdown is currently active */
	UFUNCTION(BlueprintCallable, Category="Countdown")
	bool IsCountdownActive() const { return bIsActive; }

	/** Returns current remaining time */
	UFUNCTION(BlueprintCallable, Category="Countdown")
	float GetRemainingTime() const { return RemainingTime; }

	/** Fired every second with remaining time — bind this in Widget Blueprint */
	UPROPERTY(BlueprintAssignable, Category="Countdown")
	FOnCountdownTick OnCountdownTick;

	/** Fired when countdown reaches zero — bind this to kill the player */
	UPROPERTY(BlueprintAssignable, Category="Countdown")
	FOnCountdownFinished OnCountdownFinished;

private:

	void Tick();

	FTimerHandle CountdownTimerHandle;
	float RemainingTime;
	bool bIsActive;
};
