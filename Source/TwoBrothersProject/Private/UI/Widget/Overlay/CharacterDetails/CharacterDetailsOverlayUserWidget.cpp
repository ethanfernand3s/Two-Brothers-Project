// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Overlay/CharacterDetails/CharacterDetailsOverlayUserWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UCharacterDetailsOverlayUserWidget::OnAttributePointsUpdated(int32 NewAttributePoints)
{
	if (!IsValid(Image_IconFill)) return;
	
	if (NewAttributePoints > 0)
	{
		if (bIsAttributePointsEmpty)
		{
			Image_IconFill->SetBrushTintColor(FLinearColor::White);
			bIsAttributePointsEmpty = false;
		}
	}
	else
	{
		Image_IconFill->SetBrushTintColor(FLinearColor(0.2f, 0.2f, 0.2f, 1.0f));
		bIsAttributePointsEmpty = true;
	}
}

void UCharacterDetailsOverlayUserWidget::OnCharacterIconChanged(UTexture2D* CharacterIcon)
{
	if (!IsValid(CharacterIcon) || !IsValid(Image_Icon)) return;
	Image_Icon->SetBrushFromTexture(CharacterIcon);
}

void UCharacterDetailsOverlayUserWidget::OnLevelUpdated(int32 NewLevel)
{
	if (!IsValid(TextBlock_Level)) return;
	TextBlock_Level->SetText(FText::AsNumber(NewLevel));
}

void UCharacterDetailsOverlayUserWidget::OnXPPercentageUpdated(float NewXPPercentage)
{
	if (!IsValid(ProgressBar_XP)) return;
	ProgressBar_XP->SetPercent(NewXPPercentage);
}

void UCharacterDetailsOverlayUserWidget::OnHealthUpdated(float NewHealth)
{
	if (!IsValid(ProgressBar_Health)) return;
	Health = NewHealth;

	if (MaxHealth > 0)
	{
		ProgressBar_Health->SetPercent(Health / MaxHealth);
	}
}

void UCharacterDetailsOverlayUserWidget::OnMaxHealthUpdated(float NewMaxHealth)
{
	if (!IsValid(ProgressBar_Health)) return;
	MaxHealth = NewMaxHealth;

	if (MaxHealth > 0)
	{
		ProgressBar_Health->SetPercent(Health / MaxHealth);
	}
}

void UCharacterDetailsOverlayUserWidget::OnEnergyUpdated(float NewEnergy)
{
	if (!IsValid(ProgressBar_Energy)) return;
	Energy = NewEnergy;

	if (MaxEnergy > 0)
	{
		ProgressBar_Energy->SetPercent(Energy / MaxEnergy);
	}
}

void UCharacterDetailsOverlayUserWidget::OnMaxEnergyUpdated(float NewMaxEnergy)
{
	if (!IsValid(ProgressBar_Energy)) return;
	MaxEnergy = NewMaxEnergy;

	if (MaxEnergy > 0)
	{
		ProgressBar_Energy->SetPercent(Energy / MaxEnergy);
	}
}
