// Copyright Epic Games, Inc. All Rights Reserved.

#include "InteractiveDamagePickup.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GE_DamageHealth.h"

AInteractiveDamagePickup::AInteractiveDamagePickup()
{
	DamageEffect = UGE_DamageHealth::StaticClass();
}

void AInteractiveDamagePickup::Interact_Implementation(AActor* Interactor)
{
	if (!IInteractableInterface::Execute_CanInteract(this, Interactor))
	{
		return;
	}

	// Apply the damage effect to the interactor's ASC
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Interactor))
	{
		if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
		{
			if (DamageEffect)
			{
				FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
				Context.AddSourceObject(this);
				FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(DamageEffect, 1, Context);
				if (Spec.IsValid())
				{
					ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
				}
			}
		}
	}

	Destroy();
}
