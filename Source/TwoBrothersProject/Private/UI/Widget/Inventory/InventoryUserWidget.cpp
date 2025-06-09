// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Inventory/InventoryUserWidget.h"


#include "Kismet/GameplayStatics.h"
#include "UI/HUD/PlayerHUD.h"
#include "UI/Widget/Inventory/AttributeMenuUserWidget.h"
#include "UI/WidgetController/InventoryWidgetController.h"

void UInventoryUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PC->GetHUD()))
		{
			auto* InventoryWidgetController = PlayerHUD->GetInventoryWidgetController(PC);
			AttributeMenu->SetWidgetController(InventoryWidgetController);
			SetWidgetController(InventoryWidgetController);
			InventoryWidgetController->BroadcastInitialValues();
		}
	}
	if (!(WidgetController == nullptr))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "NOT NULL");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "TRASH");
	}
}
