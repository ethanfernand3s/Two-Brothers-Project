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
		if (OverlayWidgetController->IsAnimalInhabited()) HideSecondaryInfo();
		OverlayWidgetController->AttributePointsChanged.AddUObject(this,&UOverlayUserWidget::OnAttributePointsUpdated);
		OverlayWidgetController->CharacterIconChanged.AddUObject(this,&UOverlayUserWidget::OnCharacterIconChanged);
		OverlayWidgetController->XPChanged.AddUObject(this,&UOverlayUserWidget::OnXPPercentageUpdated);
		OverlayWidgetController->LevelChanged.AddUObject(this,&UOverlayUserWidget::OnLevelUpdated);

		OverlayWidgetController->HealthChanged.AddUObject(this,&UOverlayUserWidget::OnHealthUpdated);
		OverlayWidgetController->MaxHealthChanged.AddUObject(this,&UOverlayUserWidget::OnMaxHealthUpdated);
		OverlayWidgetController->EnergyChanged.AddUObject(this,&UOverlayUserWidget::OnEnergyUpdated);
		OverlayWidgetController->MaxEnergyChanged.AddUObject(this,&UOverlayUserWidget::OnMaxEnergyUpdated);
	}
}

void UOverlayUserWidget::OnAttributePointsUpdated(int32 NewAttributePoints, bool bIsParasiteVal)
{
	if (!IsValid(OverlayWidgetController)) return;
	UTextBlock* FocusedTextBlock = nullptr;
	UImage* FocusedIconFill = nullptr;
	if (OverlayWidgetController->IsAnimalInhabited())
	{
		// Parasite becomes secondary if animal is inhabited
		FocusedTextBlock = bIsParasiteVal ? TextBlock_PointsToSpend_Secondary : TextBlock_PointsToSpend_Primary;
		FocusedIconFill = bIsParasiteVal ? Image_Icon_Secondary_Fill : Image_Icon_Primary_Fill;
	}
	else
	{
		// Otherwise parasite is primary
		FocusedTextBlock = TextBlock_PointsToSpend_Primary;
		FocusedIconFill = Image_Icon_Primary_Fill;
	}
	if (!IsValid(FocusedTextBlock) || !IsValid(FocusedIconFill)) return;
	
	if (NewAttributePoints > 0)
	{
		if (!FocusedTextBlock->IsVisible() || !FocusedIconFill->IsVisible())
		{
			FocusedTextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			FocusedIconFill->SetBrushTintColor(FLinearColor::White);
		}
		FocusedTextBlock->SetText(FText::AsNumber(NewAttributePoints));
	}
	else
	{
		FocusedTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		FocusedIconFill->SetBrushTintColor(FLinearColor(0.2f, 0.2f, 0.2f, 1.0f));
	}
}

void UOverlayUserWidget::OnCharacterIconChanged(UTexture2D* CharacterIcon, bool bIsParasiteVal)
{
	if (!IsValid(CharacterIcon)) return;
	
	UImage* FocusedIcon = nullptr;
	if (OverlayWidgetController->IsAnimalInhabited())
	{
		// Parasite becomes secondary if animal is inhabited
		FocusedIcon = bIsParasiteVal ? Image_Icon_Secondary : Image_Icon_Primary;
	}
	else
	{
		// Otherwise parasite is primary
		FocusedIcon = Image_Icon_Primary;
	}
	if (!IsValid(FocusedIcon)) return;
	FocusedIcon->SetBrushFromTexture(CharacterIcon);
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

void UOverlayUserWidget::HideSecondaryInfo() const
{
	Image_Icon_Secondary->SetVisibility(ESlateVisibility::Hidden);
	Image_Icon_Secondary_Fill->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_PointsToSpend_Secondary->SetVisibility(ESlateVisibility::Hidden);
}
