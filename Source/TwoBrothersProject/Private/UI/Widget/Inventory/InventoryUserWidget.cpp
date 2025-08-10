// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Inventory/InventoryUserWidget.h"


#include "Blueprint/WidgetTree.h"
#include "UI/Widget/Inventory/Abilities/AbilityCardUserWidget.h"
#include "UI/Widget/Inventory/Abilities/PassiveAbilityDropdownUserWidget.h"
#include "UI/Widget/Inventory/Items/ItemsPanelUserWidget.h"
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
	switch (AbilityInfo.AbilityType)
	{
	case EAbilityType::Default:
		{
			UAbilityCardUserWidget* NewCard = WidgetTree->ConstructWidget<UAbilityCardUserWidget>(Square_AbilityCardUserWidgetClass);
			NewCard->SetCardData(AbilityInfo);

			if (AbilityInfo.bIsOnHotbar)
			{
				StatsPanel->ReceiveDefaultAbility(NewCard);
			}
			else
			{
				ItemsPanel->ReceiveNewCard(NewCard);
			}
			break;
		}
	case EAbilityType::Passive:
		{
			UAbilityCardUserWidget* NewCard = WidgetTree->ConstructWidget<UAbilityCardUserWidget>(Circle_AbilityCardUserWidgetClass);
			NewCard->SetCardData(AbilityInfo);
			
			if (AbilityInfo.bIsOnHotbar)
			{
				StatsPanel->PassiveAbilityDropdown->ReceivePassiveAbility(NewCard);
			}
			else
			{
				ItemsPanel->ReceiveNewCard(NewCard);
			}
			break;
		};
	case EAbilityType::Main:
		{
			UAbilityCardUserWidget* NewCard = WidgetTree->ConstructWidget<UAbilityCardUserWidget>(Diamond_AbilityCardUserWidgetClass);
			NewCard->SetCardData(AbilityInfo);
			if (AbilityInfo.bIsOnHotbar)
			{
				StatsPanel->ReceiveMainAbility(NewCard);
			}
			else
			{
				// TODO: Implement this
				ItemsPanel->ReceiveNewCard(NewCard);
			}
			break;
		};
	default:
		{
			break;
		}
	}
}
