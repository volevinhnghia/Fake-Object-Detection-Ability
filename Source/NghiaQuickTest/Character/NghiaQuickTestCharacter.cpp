// Copyright Epic Games, Inc. All Rights Reserved.

#include "NghiaQuickTestCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "BaseAttributeSet.h"
#include "GameplayEffect.h"
#include "NghiaQuickTest.h"

ANghiaQuickTestCharacter::ANghiaQuickTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Create the Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// Create the base attribute set (automatically registered with the ASC)
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));

	// Default attribute values (editable in Blueprint)
	DefaultHealth = 100.0f;
	DefaultMaxHealth = 100.0f;
	DefaultMana = 50.0f;
	DefaultMaxMana = 50.0f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

UAbilitySystemComponent* ANghiaQuickTestCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ANghiaQuickTestCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the ASC with self as owner and avatar
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		GiveDefaultAbilities();

		// Apply editable default attribute values
		if (AttributeSet)
		{
			AttributeSet->InitHealth(DefaultHealth);
			AttributeSet->InitMaxHealth(DefaultMaxHealth);
			AttributeSet->InitMana(DefaultMana);
			AttributeSet->InitMaxMana(DefaultMaxMana);
		}

		// Listen for Health reaching zero
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UBaseAttributeSet::GetHealthAttribute()).AddUObject(this, &ANghiaQuickTestCharacter::OnHealthChanged);

		// Apply the optional initial stat effect (override default attribute values from Blueprint)
		if (DefaultStatEffect)
		{
			FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
			Context.AddSourceObject(this);
			FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(DefaultStatEffect, 1, Context);
			if (Spec.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
			}
		}
	}
}

void ANghiaQuickTestCharacter::GiveDefaultAbilities()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		if (AbilityClass)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
		}
	}
}

void ANghiaQuickTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANghiaQuickTestCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ANghiaQuickTestCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANghiaQuickTestCharacter::Look);

		// Interact (E)
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ANghiaQuickTestCharacter::OnInteractPressed);

		// Reveal (Q) — hold to reveal, release to stop
		EnhancedInputComponent->BindAction(RevealAction, ETriggerEvent::Started, this, &ANghiaQuickTestCharacter::OnRevealPressed);
		EnhancedInputComponent->BindAction(RevealAction, ETriggerEvent::Completed, this, &ANghiaQuickTestCharacter::OnRevealReleased);
	}
	else
	{
		UE_LOG(LogNghiaQuickTest, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ANghiaQuickTestCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ANghiaQuickTestCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ANghiaQuickTestCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ANghiaQuickTestCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ANghiaQuickTestCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ANghiaQuickTestCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void ANghiaQuickTestCharacter::OnInteractPressed()
{
	if (AbilitySystemComponent && InteractAbilityClass)
	{
		AbilitySystemComponent->TryActivateAbilityByClass(InteractAbilityClass);
	}
}

void ANghiaQuickTestCharacter::OnRevealPressed()
{
	if (AbilitySystemComponent && RevealAbilityClass)
	{
		AbilitySystemComponent->TryActivateAbilityByClass(RevealAbilityClass);
	}
}

void ANghiaQuickTestCharacter::OnRevealReleased()
{
	if (AbilitySystemComponent && RevealAbilityClass)
	{
		if (FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromClass(RevealAbilityClass))
		{
			AbilitySystemComponent->CancelAbilityHandle(Spec->Handle);
		}
	}
}

void ANghiaQuickTestCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0.0f)
	{
		OnDeath();
	}
}

void ANghiaQuickTestCharacter::OnDeath_Implementation()
{
	DisableInput(Cast<APlayerController>(GetController()));
}

void ANghiaQuickTestCharacter::KillCharacter()
{
	if (AttributeSet && AbilitySystemComponent)
	{
		// Create instant GE that sets Health to 0
		UGameplayEffect* KillEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("KillEffect")));
		KillEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

		FGameplayModifierInfo Modifier;
		Modifier.Attribute = UBaseAttributeSet::GetHealthAttribute();
		Modifier.ModifierOp = EGameplayModOp::Override;
		FScalableFloat ZeroValue;
		ZeroValue.Value = 0.0f;
		Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(ZeroValue);
		KillEffect->Modifiers.Add(Modifier);

		FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
		AbilitySystemComponent->ApplyGameplayEffectToSelf(KillEffect, 1, Context);
	}
}
