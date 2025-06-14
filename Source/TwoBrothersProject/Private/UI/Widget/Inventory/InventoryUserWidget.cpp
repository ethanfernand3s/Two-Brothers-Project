// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Inventory/InventoryUserWidget.h"


#include "Kismet/GameplayStatics.h"
#include "UI/HUD/PlayerHUD.h"
#include "UI/Widget/Inventory/AttributeMenuUserWidget.h"
#include "UI/WidgetController/InventoryWidgetController.h"

void UInventoryUserWidget::SetWidgetController(UObject* InWidgetController)
{
	Super::SetWidgetController(InWidgetController);

	AttributeMenu->SetWidgetController(InWidgetController);
}
