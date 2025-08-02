// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Inventory/InventoryUserWidget.h"


#include "Blueprint/WidgetTree.h"
#include "UI/Widget/Inventory/Abilities/AbilityCardUserWidget.h"
#include "UI/Widget/Inventory/Stats/StatsPanelUserWidget.h"
#include "UI/WidgetController/InventoryWidgetController.h"

void UInventoryUserWidget::SetWidgetController(UObject* InWidgetController)
{
	StatsPanel->SetWidgetController(InWidgetController);
	
	Super::SetWidgetController(InWidgetController);
}

void UInventoryUserWidget::OnWidgetControllerSet()
{
	Super::OnWidgetControllerSet();

	if(InventoryWidgetController = Cast<UInventoryWidgetController>(WidgetController))
	{
		InventoryWidgetController->AbilityInfoDelegate.AddUObject(this, &UInventoryUserWidget::OnAbilityInfoRecieved);
	}
}

void UInventoryUserWidget::OnAbilityInfoRecieved(const FTBAbilityInfo& AbilityInfo)
{
	// Create widget
	UAbilityCardUserWidget* NewCard = WidgetTree->ConstructWidget<UAbilityCardUserWidget>(UAbilityCardUserWidget::StaticClass());
	NewCard
	// Decide if it goes the inventory or stats panel and type of ability

	// Set it

	// Add to appropriate grid
}
