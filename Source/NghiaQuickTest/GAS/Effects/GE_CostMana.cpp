// Copyright Epic Games, Inc. All Rights Reserved.

#include "GE_CostMana.h"
#include "BaseAttributeSet.h"

UGE_CostMana::UGE_CostMana()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo Modifier;
	Modifier.Attribute = UBaseAttributeSet::GetManaAttribute();
	Modifier.ModifierOp = EGameplayModOp::Additive;

	// -10 Mana by default
	FScalableFloat ManaValue;
	ManaValue.Value = -10.0f;
	Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(ManaValue);

	Modifiers.Add(Modifier);
}
