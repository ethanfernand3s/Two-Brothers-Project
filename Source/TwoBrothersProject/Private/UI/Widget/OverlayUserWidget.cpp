// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/OverlayUserWidget.h"

#include "Components/ProgressBar.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UOverlayUserWidget::OnWidgetControllerSet()
{
	if (auto* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		OverlayWidgetController->OnHealthChanged.AddUObject(this, &UOverlayUserWidget::OnHealthChanged);
		OverlayWidgetController->OnMaxHealthChanged.AddUObject(this, &UOverlayUserWidget::OnMaxHealthChanged);
	}
}

void UOverlayUserWidget::OnHealthChanged(float NewHealth) 
{
	Health = NewHealth;
	SetHealthBarPercentage();
}

void UOverlayUserWidget::OnMaxHealthChanged(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
	SetHealthBarPercentage();
}

void UOverlayUserWidget::SetHealthBarPercentage()
{
	if (MaxHealth != 0)
	{
		const float HealthPercent = Health / MaxHealth;
		ProgressBar_Health->SetPercent(HealthPercent);
	}
}
