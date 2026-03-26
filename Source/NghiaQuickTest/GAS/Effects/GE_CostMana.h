// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_CostMana.generated.h"

/**
 * Instant GameplayEffect that subtracts Mana.
 * Default: -10 Mana. Override in Blueprint children for different costs.
 */
UCLASS(Blueprintable)
class UGE_CostMana : public UGameplayEffect
{
	GENERATED_BODY()

public:

	UGE_CostMana();
};
