// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Logging/LogMacros.h"
#include "NghiaQuickTestCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UAbilitySystemComponent;
class UBaseAttributeSet;
class UGameplayAbility;
class UGameplayEffect;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 *  Implements IAbilitySystemInterface for Gameplay Ability System
 */
UCLASS(abstract)
class ANghiaQuickTestCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Ability System Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Abilities", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	/** Base attribute set (Health, Mana, Stamina, AttackPower, etc.) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Abilities", meta = (AllowPrivateAccess = "true"))
	UBaseAttributeSet* AttributeSet;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	/** Default abilities granted to this character on BeginPlay */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	/** Optional GameplayEffect applied at BeginPlay to initialize attribute values (e.g. set Health to 200) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities")
	TSubclassOf<UGameplayEffect> DefaultStatEffect;

public:

	/** Constructor */
	ANghiaQuickTestCharacter();

	//~ IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Returns the base attribute set */
	UFUNCTION(BlueprintCallable, Category="Abilities")
	UBaseAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:

	virtual void BeginPlay() override;

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Grant default abilities to this character */
	void GiveDefaultAbilities();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
