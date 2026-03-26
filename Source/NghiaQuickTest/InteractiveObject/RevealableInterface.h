// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RevealableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class URevealableInterface : public UInterface
{
	GENERATED_BODY()
};

class IRevealableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void SetRevealEnabled(bool bEnabled);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void ForceHideReveal();
};
