// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Inventory/Slots/SlotPanelUserWidget.h"

#include "UI/Widget/Inventory/Slots/SlotContainerUserWidget.h"

void USlotPanelUserWidget::OnWidgetControllerSet()
{
	Super::OnWidgetControllerSet();

	SlotContainer->SetWidgetController(WidgetController);
}
