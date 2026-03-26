// Copyright Epic Games, Inc. All Rights Reserved.

#include "GE_DamageHealth.h"
#include "BaseAttributeSet.h"

UGE_DamageHealth::UGE_DamageHealth()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo Modifier;
	Modifier.Attribute = UBaseAttributeSet::GetHealthAttribute();
	Modifier.ModifierOp = EGameplayModOp::Additive;

	// -10 HP by default
	FScalableFloat DamageValue;
	DamageValue.Value = -10.0f;
	Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(DamageValue);

	Modifiers.Add(Modifier);
}
