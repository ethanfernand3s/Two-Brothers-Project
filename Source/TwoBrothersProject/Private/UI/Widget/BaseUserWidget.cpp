// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/BaseUserWidget.h"

#include "UI/WidgetController/BaseWidgetController.h"

void UBaseUserWidget::SetWidgetController(UBaseWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetController->OnWidgetControllerRebound.AddUObject(this, &ThisClass::OnWidgetControllerRebound);
	OnWidgetControllerSet();
}

void UBaseUserWidget::OnWidgetControllerSet()
{
	// Implemented In Children
}

void UBaseUserWidget::OnWidgetControllerRebound(bool bIsAnimalInhabited)
{
	// Implemented In Children
}
