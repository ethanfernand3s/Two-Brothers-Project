// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Overlay/OverlayUserWidget.h"

#include "Components/ProgressBar.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UOverlayUserWidget::OnWidgetControllerSet()
{
	if (auto* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		
	}
}
