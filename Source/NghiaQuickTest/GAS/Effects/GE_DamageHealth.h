// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_DamageHealth.generated.h"

/**
 * Instant GameplayEffect that subtracts Health.
 * Default: -10 HP. Override DamageAmount in Blueprint children for different values.
 */
UCLASS(Blueprintable)
class UGE_DamageHealth : public UGameplayEffect
{
	GENERATED_BODY()

public:

	UGE_DamageHealth();
};
