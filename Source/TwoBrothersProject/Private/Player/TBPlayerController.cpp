// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoBrothersProject/Public/Player/TBPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "TBGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Camera/ParasiteCameraManager.h"
#include "Characters/BaseAnimalCharacter.h"
#include "Characters/ParasiteCharacter.h"
#include "Input/TBInputComponent.h"
#include "Player/ParasitePlayerState.h"


ATBPlayerController::ATBPlayerController()
{
	PlayerCameraManagerClass = AParasiteCameraManager::StaticClass();
}
void ATBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UTBInputComponent* NewInput = NewObject<UTBInputComponent>(this, UTBInputComponent::StaticClass(), TEXT("TBInputComponent"));
	NewInput->RegisterComponent();
	InputComponent = NewInput;
	
	UTBInputComponent* TBInputComponent = CastChecked<UTBInputComponent>(InputComponent);
	TBInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATBPlayerController::Move);
	TBInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATBPlayerController::Look);

	TBInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ATBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(InputContext, 0);
	}
}

void ATBPlayerController::Move(const FInputActionValue& InputActionValue)
{
	//if (!bPawnAlive) return; -> Add Later
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ATBPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	AddYawInput(InputAxisVector.X);
	AddPitchInput(InputAxisVector.Y);
}

void ATBPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
		GetASC()->AbilityInputTagPressed(InputTag);
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void ATBPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
		GetASC()->AbilityInputTagReleased(InputTag);
	
}

void ATBPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);
}

UBaseAbilitySystemComponent* ATBPlayerController::GetASC() 
{
	if (GetPawn()->IsA(AParasiteCharacter::StaticClass()))
	{
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(GetPlayerState<AParasitePlayerState>()->GetAbilitySystemComponent());
	}
	else
	{
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(Cast<ABaseAnimalCharacter>(GetPawn())->GetAbilitySystemComponent());
	}
	return BaseAbilitySystemComponent;
}

