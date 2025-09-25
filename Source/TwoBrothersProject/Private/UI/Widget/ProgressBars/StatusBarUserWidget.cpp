// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/ProgressBars/StatusBarUserWidget.h"

#include "AbilitySystem/Base/BaseAttributeSet.h"
#include "Components/ProgressBar.h"

void UStatusBarUserWidget::RefreshHealth()
{
	if (!AttributeSet) return;

	// Get health values
	const float CurrentHealth = AttributeSet->GetHealth();
	const float MaxHealth = AttributeSet->GetMaxHealth();

	// Avoid divide by zero
	float HealthPercent = 0.f;
	if (MaxHealth > 0.f)
	{
		HealthPercent = FMath::Clamp(CurrentHealth / MaxHealth, 0.f, 1.f);
	}

	// Update progress bar
	if (ProgressBar_HealthBar)
	{
		ProgressBar_HealthBar->SetPercent(HealthPercent);
	}
}
