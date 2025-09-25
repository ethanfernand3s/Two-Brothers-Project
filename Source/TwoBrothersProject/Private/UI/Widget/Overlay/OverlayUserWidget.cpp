#include "UI/Widget/Overlay/OverlayUserWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UOverlayUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UOverlayUserWidget::OnWidgetControllerSet()
{
	if ((OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController)))
	{
		OverlayWidgetController->AttributePointsChanged.AddUObject(this,&UOverlayUserWidget::OnAttributePointsUpdated);
		OverlayWidgetController->XPChanged.AddUObject(this,&UOverlayUserWidget::OnXPPercentageUpdated);
		OverlayWidgetController->LevelChanged.AddUObject(this,&UOverlayUserWidget::OnLevelUpdated);

		OverlayWidgetController->HealthChanged.AddUObject(this,&UOverlayUserWidget::OnHealthUpdated);
		OverlayWidgetController->MaxHealthChanged.AddUObject(this,&UOverlayUserWidget::OnMaxHealthUpdated);
		OverlayWidgetController->EnergyChanged.AddUObject(this,&UOverlayUserWidget::OnEnergyUpdated);
		OverlayWidgetController->MaxEnergyChanged.AddUObject(this,&UOverlayUserWidget::OnMaxEnergyUpdated);
	}
}

void UOverlayUserWidget::OnAttributePointsUpdated(int32 NewAttributePoints)
{
	if (NewAttributePoints > 0)
	{
		if (bIsAttributePointsEmpty)
		{
			TextBlock_PointsToSpend_Primary->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Image_Icon_Primary_Fill->SetBrushTintColor(FLinearColor::White);
			bIsAttributePointsEmpty = false;
		}
		TextBlock_PointsToSpend_Primary->SetText(FText::AsNumber(NewAttributePoints));
	}
	else
	{
		TextBlock_PointsToSpend_Primary->SetVisibility(ESlateVisibility::Collapsed);
		Image_Icon_Primary_Fill->SetBrushTintColor(FLinearColor(0.2f, 0.2f, 0.2f, 1.0f));
		bIsAttributePointsEmpty = true;
	}

	// TODO: Update Primary (for parasite) if host is present and primary for the host
}

void UOverlayUserWidget::OnLevelUpdated(int32 NewLevel)
{
	TextBlock_Level->SetText(FText::AsNumber(NewLevel));
}

void UOverlayUserWidget::OnXPPercentageUpdated(float NewXPPercentage)
{
	ProgressBar_XP->SetPercent(NewXPPercentage);
}

void UOverlayUserWidget::OnHealthUpdated(float NewHealth)
{
	Health = NewHealth;

	if (MaxHealth > 0)
	{
		ProgressBar_Health->SetPercent(Health / MaxHealth);
	}
}

void UOverlayUserWidget::OnMaxHealthUpdated(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;

	if (MaxHealth > 0)
	{
		ProgressBar_Health->SetPercent(Health / MaxHealth);
	}
}

void UOverlayUserWidget::OnEnergyUpdated(float NewEnergy)
{
	Energy = NewEnergy;

	if (MaxEnergy > 0)
	{
		ProgressBar_Energy->SetPercent(Energy / MaxEnergy);
	}
}

void UOverlayUserWidget::OnMaxEnergyUpdated(float NewMaxEnergy)
{
	MaxEnergy = NewMaxEnergy;

	if (MaxEnergy > 0)
	{
		ProgressBar_Energy->SetPercent(Energy / MaxEnergy);
	}
}