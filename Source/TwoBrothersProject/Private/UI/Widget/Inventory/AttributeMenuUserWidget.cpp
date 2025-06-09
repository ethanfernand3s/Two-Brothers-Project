// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Inventory/AttributeMenuUserWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/Widget/Inventory/AttributeRowUserWidget.h"
#include "UI/Widget/Inventory/AttributeXPBar.h"
#include "UI/WidgetController/InventoryWidgetController.h"

void UAttributeMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAttributeMenuUserWidget::SetWidgetController(UObject* InWidgetController)
{
	AttributeRow_Health->SetWidgetController(InWidgetController);
	AttributeRow_Defense->SetWidgetController(InWidgetController);
	AttributeRow_Drowsiness->SetWidgetController(InWidgetController);
	AttributeRow_Hunger->SetWidgetController(InWidgetController);
	AttributeRow_Thirst->SetWidgetController(InWidgetController);
	AttributeRow_Oxygen->SetWidgetController(InWidgetController);
	AttributeRow_Speed->SetWidgetController(InWidgetController);
	AttributeRow_Stamina->SetWidgetController(InWidgetController);
	AttributeRow_Strength->SetWidgetController(InWidgetController);
	AttributeXPBar->SetWidgetController(InWidgetController);
	
	Super::SetWidgetController(InWidgetController);
	
}

void UAttributeMenuUserWidget::OnWidgetControllerSet()
{
	if(auto* InventoryWidgetController = Cast<UInventoryWidgetController>(WidgetController))
	{
		InventoryWidgetController->Health_AttributeInfoDelegate.AddUObject(this,&UAttributeMenuUserWidget::SetHealthAttributeRow);
	}
}

void UAttributeMenuUserWidget::SetHealthAttributeRow(const FTBFloatAttributeInfo& CurrentHealthInfo,
	const FTBFloatAttributeInfo& MaxHealthInfo)
{
	const float CurrentHealth{CurrentHealthInfo.AttributeValue}, MaxHealth{MaxHealthInfo.AttributeValue};
	
	if (MaxHealth != 0)
	{
		FText FormattedHealth = FText::Format(
	NSLOCTEXT("HealthUI", "HealthFormat", "{0} / {1}"),
		FText::AsNumber(CurrentHealth),
		FText::AsNumber(MaxHealth));
		
		AttributeRow_Health->TextBlock_AttributeRatio->SetText(FormattedHealth);
		const float HealthPercent = CurrentHealth / MaxHealth;
		AttributeRow_Health->ProgressBar_Attribute->SetPercent(HealthPercent);
	}
}