#include "UI/HUD/PlayerHUD.h"

#include "AbilitySystem/Animal/AnimalAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/Components/TBInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TBPlayerController.h"
#include "Player/ParasitePlayerState.h"
#include "UI/Widget/Overlay/OverlayUserWidget.h"
#include "UI/Widget/Inventory/InventoryUserWidget.h"
#include "UI/WidgetController/InventoryWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Animal/AnimalAttributeSet.h"

void APlayerHUD::InitUI(ATBPlayerController* PC)
{
	if (!IsValid(PC)) return;
	
	if (WCParams == nullptr) SetupWidgetParams(PC);
	else UpdateAnimalWidgetParams(PC);

	if (OverlayWidgetController != nullptr)
	{
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->RebindToDependencies();
	}
	if (InventoryWidgetController != nullptr)
	{
		InventoryWidgetController->SetWidgetControllerParams(WCParams);
		InventoryWidgetController->RebindToDependencies();
	}
	
	InitOverlay(PC);
	
	if (IsValid(PC->GetInventoryComponent()) && PC->GetInventoryComponent()->GetInventoryWidget() == nullptr)
	{
		PC->GetInventoryComponent()->SetInventoryWidget(InitInventory(PC));
	}

	// Setup Input
	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->SetShowMouseCursor(false);
}

void APlayerHUD::InitOverlay(ATBPlayerController* PC)
{
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, fill out BP_PlayerHUD"));
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, fill out BP_PlayerHUD"));

	if (OverlayUserWidget == nullptr)
	{
		// Create Widget
		OverlayUserWidget = CreateWidget<UOverlayUserWidget>(GetWorld(), OverlayWidgetClass);
		if (!IsValid(OverlayUserWidget)) return;

		// Setup Widget Params
		OverlayWidgetController = GetOverlayWidgetController();
		OverlayUserWidget->SetWidgetController(OverlayWidgetController);
		OverlayWidgetController->BroadcastInitialValues();

		// Setup Viewport
		OverlayUserWidget->AddToViewport();
	}
}

UOverlayWidgetController* APlayerHUD::GetOverlayWidgetController()
{ 
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UInventoryUserWidget* APlayerHUD::InitInventory(ATBPlayerController* PC)
{
	check(InventoryWidgetControllerClass);
	check(InventoryUserWidgetClass);
	
	if (!IsValid(PC)) return nullptr;

	// Create Widget
	UInventoryUserWidget* InventoryUserWidget = CreateWidget<UInventoryUserWidget>(PC, InventoryUserWidgetClass);
	if (!IsValid(InventoryUserWidget)) return nullptr;

	// Setup Widget Params
	InventoryWidgetController = GetInventoryWidgetController();
	InventoryUserWidget->SetWidgetController(InventoryWidgetController);
	InventoryWidgetController->BroadcastInitialValues();

	// Setup Viewport
	InventoryUserWidget->AddToViewport();
	InventoryUserWidget->SetVisibility(ESlateVisibility::Collapsed);

	return InventoryUserWidget;
}

UInventoryWidgetController* APlayerHUD::GetInventoryWidgetController()
{
	if (InventoryWidgetController == nullptr)
	{
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetWidgetControllerParams(WCParams);
		InventoryWidgetController->BindCallbacksToDependencies();
	}
	return InventoryWidgetController;
}

void APlayerHUD::SetupWidgetParams(ATBPlayerController* PC)
{
	/* Immutable */
	AParasitePlayerState* PS = Cast<AParasitePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	UParasiteAbilitySystemComponent* ParasiteASC = Cast<UParasiteAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	UParasiteAttributeSet* ParasiteAS = PS->GetParasiteAttributeSet();
	ATBPlayerController* TBPC = Cast<ATBPlayerController>(PC);
	WCParams = MakeUnique<FWidgetControllerParams>(TBPC, PS, ParasiteASC, ParasiteAS, Cast<IPlayerInterface>(PS));
	if (!IsValid(TBPC) || !WCParams.IsValid()) return;

	/* Mutable */
	UpdateAnimalWidgetParams(TBPC);
}

void APlayerHUD::UpdateAnimalWidgetParams(ATBPlayerController* TBPC)
{
	if (!WCParams.IsValid()) return;
	
	if (IsValid(TBPC) && TBPC->GetIsAnimalPossessed())
	{
		WCParams->AnimalASC = Cast<UAnimalAbilitySystemComponent>(TBPC->GetASC());
		WCParams->AnimalAS = TBPC->GetCurrentAnimalsAttributeSet();
		WCParams->AnimalPI = TBPC->GetActivePlayerInterface();
	}
	
}
