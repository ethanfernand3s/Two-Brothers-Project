// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoBrothersProject/Public/Player/TBPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "TBGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Animal/AnimalAttributeSet.h"
#include "Camera/ParasiteCameraManager.h"
#include "Characters/BaseAnimalCharacter.h"
#include "Characters/ParasiteCharacter.h"
#include "Input/TBInputComponent.h"
#include "Player/ParasitePlayerState.h"
#include "UI/HUD/PlayerHUD.h"


ATBPlayerController::ATBPlayerController()
{
	PlayerCameraManagerClass = AParasiteCameraManager::StaticClass();
	bIsAnimalPossessed = false;
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
	TBInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ATBPlayerController::Inventory);

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
	OnPossessedPawnChanged.AddDynamic(this, &ATBPlayerController::OnPawnChanged);
}

void ATBPlayerController::OnPawnChanged(APawn* InOldPawn, APawn* InNewPawn)
{
	if (InOldPawn && InOldPawn->IsA(AParasiteCharacter::StaticClass()))
	{
		bIsAnimalPossessed = true;
	}
	else if (InNewPawn && InNewPawn->IsA(AParasiteCharacter::StaticClass()))
	{
		bIsAnimalPossessed = false;
	}
}

bool ATBPlayerController::GetIsAnimalPossessed() const
{
	return bIsAnimalPossessed;
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

void ATBPlayerController::Inventory()
{
	Cast<APlayerHUD>(GetHUD())->Inventory(this);
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
	if (bIsAnimalPossessed)
	{
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(Cast<ABaseAnimalCharacter>(GetPawn())->GetAbilitySystemComponent());
	}
	else
	{
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(GetPlayerState<AParasitePlayerState>()->GetAbilitySystemComponent());
	}
	return BaseAbilitySystemComponent;
}

UAnimalAttributeSet* ATBPlayerController::GetCurrentAnimalsAttributeSet() const
{
	if (bIsAnimalPossessed)
	{
		return Cast<UAnimalAttributeSet>(Cast<ABaseAnimalCharacter>(GetPawn())->GetAttributeSet());
	}
	UE_LOG(LogTemp, Warning, TEXT("Trying to get animal attribute set but there is currently no animal possessed!"));

	return nullptr;
}

