// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/BaseUserWidget.h"

void UBaseUserWidget::SetWidgetController(UBaseWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}

void UBaseUserWidget::OnWidgetControllerSet()
{
	// Implemented In Children
}
