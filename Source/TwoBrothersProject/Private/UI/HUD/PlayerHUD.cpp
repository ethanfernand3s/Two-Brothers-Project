// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/PlayerHUD.h"

#include "AbilitySystem/Animal/AnimalAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Characters/ParasiteCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ParasitePlayerState.h"
#include "Player/TBPlayerController.h"
#include "UI/Widget/Overlay/OverlayUserWidget.h"
#include "UI/Widget/Inventory/InventoryUserWidget.h"
#include "UI/Widget/Possession/PossessMiniGameUserWidget.h"
#include "UI/WidgetController/InventoryWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* APlayerHUD::GetOverlayWidgetController(APlayerController* PC)
{ 
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		if (WCParams == nullptr)
		{
			SetupWidgetParams(PC);
		};
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

UInventoryWidgetController* APlayerHUD::GetInventoryWidgetController(APlayerController* PC)
{
	if (InventoryWidgetController == nullptr)
	{
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
		if (WCParams == nullptr)
		{
			SetupWidgetParams(PC);
		}
		InventoryWidgetController->SetWidgetControllerParams(WCParams);
		InventoryWidgetController->BindCallbacksToDependencies();
	}
	return InventoryWidgetController;
}

void APlayerHUD::SetupWidgetParams(APlayerController* PC)
{
	WCParams = MakeUnique<FWidgetControllerParams>();
	AParasitePlayerState* PS = Cast<AParasitePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	UParasiteAbilitySystemComponent* ParasiteASC = Cast<UParasiteAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	UParasiteAttributeSet* PAS = PS->GetParasiteAttributeSet();

	WCParams->TBPlayerController = PC;
	WCParams->ParasitePS = PS;
	WCParams->ParasiteAsc = ParasiteASC;
	WCParams->ParasiteAttributeSet = PAS;

	ATBPlayerController* TBPC = Cast<ATBPlayerController>(PC);
	if (TBPC->GetIsAnimalPossessed())
	{
		WCParams->AnimalAsc = Cast<UAnimalAbilitySystemComponent>(TBPC->GetASC());
		WCParams->AnimalAttributeSet = TBPC->GetCurrentAnimalsAttributeSet();
		
	}
	else
	{
		WCParams->AnimalAsc = nullptr;
		WCParams->AnimalAttributeSet = nullptr;
	}
}

void APlayerHUD::InitOverlay(APlayerController* PC)
{
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, fill out BP_PlayerHUD"));
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, fill out BP_PlayerHUD"));

	if (OverlayUserWidget == nullptr)
	{
		OverlayUserWidget = CreateWidget<UOverlayUserWidget>(GetWorld(), OverlayWidgetClass);
		OverlayWidgetController = GetOverlayWidgetController(PC);
		OverlayUserWidget->SetWidgetController(OverlayWidgetController);
		OverlayWidgetController->BroadcastInitialValues();
		
		OverlayUserWidget->AddToViewport();
	}
}

void APlayerHUD::Inventory(APlayerController* PC)
{
	checkf(InventoryWidgetControllerClass, TEXT("Inventory Widget Controller Class uninitialized, fill out BP_PlayerHUD"));
	checkf(InventoryUserWidgetClass, TEXT("Inventory Widget Class uninitialized, fill out BP_PlayerHUD"));

	if (InventoryUserWidget == nullptr)
    	{
    		InventoryUserWidget = CreateWidget<UInventoryUserWidget>(GetWorld(), InventoryUserWidgetClass);
    		InventoryWidgetController = GetInventoryWidgetController(PC);
    		InventoryUserWidget->SetWidgetController(InventoryWidgetController);
    		InventoryWidgetController->BroadcastInitialValues();
    		
    		InventoryUserWidget->AddToViewport();
    		ConfigureUIForInventory(PC, false);
    		
    		// TODO: Remove this TEST !!!
    		Cast<AParasitePlayerState>(UGameplayStatics::GetPlayerState(this, 0))->LoadProgress();
    	}
    	else
    	{
    		if (InventoryUserWidget->IsInViewport())
    		{
    			InventoryUserWidget->RemoveFromParent();
    			ConfigureUIForInventory(PC, true);
    		}
    		else
    		{
    			InventoryUserWidget->AddToViewport();
    			ConfigureUIForInventory(PC, false);
    		}
    		
    	}
	
}

void APlayerHUD::ConfigureUIForInventory(APlayerController* PC, bool bIsInViewport)
{
	if (bIsInViewport)
	{
		PC->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
	else
	{
		PC->bShowMouseCursor = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InventoryUserWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
	}
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
}
