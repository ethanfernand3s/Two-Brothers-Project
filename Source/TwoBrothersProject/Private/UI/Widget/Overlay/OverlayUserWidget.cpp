#include "UI/Widget/Overlay/OverlayUserWidget.h"

#include "UI/Widget/Overlay/CharacterDetails/CharacterDetailsOverlayUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UOverlayUserWidget::OnWidgetControllerSet()
{
	if ((OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController)))
	{
		if (OverlayWidgetController->IsAnimalInhabited()) SwapOverlayPositions(true);
		else HideAnimalDetailsOverlay();
			
		// Forward delegates straight to children via lambdas
		OverlayWidgetController->OnAttributePointsChangedDelegate.AddLambda(
			[this](int32 NewAttributePoints, bool bIsParasiteVal)
			{
				ForwardToChild<UCharacterDetailsOverlayUserWidget>(
					bIsParasiteVal, &UCharacterDetailsOverlayUserWidget::OnAttributePointsUpdated, NewAttributePoints);
			});

		OverlayWidgetController->OnCharacterIconChanged.AddLambda(
			[this](UTexture2D* NewCharacterIcon, bool bIsParasiteVal)
			{
				ForwardToChild<UCharacterDetailsOverlayUserWidget>(
					bIsParasiteVal, &UCharacterDetailsOverlayUserWidget::OnCharacterIconChanged, NewCharacterIcon);
			});

		OverlayWidgetController->OnXPChangedDelegate.AddLambda(
			[this](float NewXPPercentage, bool bIsParasiteVal)
			{
				ForwardToChild<UCharacterDetailsOverlayUserWidget>(
					bIsParasiteVal, &UCharacterDetailsOverlayUserWidget::OnXPPercentageUpdated, NewXPPercentage);
			});

		OverlayWidgetController->OnLevelChangedDelegate.AddLambda(
			[this](int32 NewLevel, bool bIsParasiteVal)
			{
				ForwardToChild<UCharacterDetailsOverlayUserWidget>(
					bIsParasiteVal, &UCharacterDetailsOverlayUserWidget::OnLevelUpdated, NewLevel);
			});

		OverlayWidgetController->HealthChanged.AddLambda(
			[this](float NewHealth, bool bIsParasiteVal)
			{
				ForwardToChild<UCharacterDetailsOverlayUserWidget>(
					bIsParasiteVal, &UCharacterDetailsOverlayUserWidget::OnHealthUpdated, NewHealth);
			});

		OverlayWidgetController->MaxHealthChanged.AddLambda(
			[this](float NewMaxHealth, bool bIsParasiteVal)
			{
				ForwardToChild<UCharacterDetailsOverlayUserWidget>(
					bIsParasiteVal, &UCharacterDetailsOverlayUserWidget::OnMaxHealthUpdated, NewMaxHealth);
			});

		OverlayWidgetController->EnergyChanged.AddLambda(
			[this](float NewEnergy, bool bIsParasiteVal)
			{
				ForwardToChild<UCharacterDetailsOverlayUserWidget>(
					bIsParasiteVal, &UCharacterDetailsOverlayUserWidget::OnEnergyUpdated, NewEnergy);
			});

		OverlayWidgetController->MaxEnergyChanged.AddLambda(
			[this](float NewMaxEnergy, bool bIsParasiteVal)
			{
				ForwardToChild<UCharacterDetailsOverlayUserWidget>(
					bIsParasiteVal, &UCharacterDetailsOverlayUserWidget::OnMaxEnergyUpdated, NewMaxEnergy);
			});
	}
}

void UOverlayUserWidget::HideAnimalDetailsOverlay()
{
	if (bPositionNeedsSwap)
	{
		SwapOverlayPositions(false);
	}
	AnimalDetailsOverlay->SetVisibility(ESlateVisibility::Collapsed);
}

void UOverlayUserWidget::SwapOverlayPositions(bool bIsAnimalInhabited)
{
	if (bIsAnimalInhabited)
	{
		// Play Anim forward
		PlayAnimation(SwapOverlayPositionsAnim);
		bPositionNeedsSwap = true;
	}
	else
	{
		// Play Anim reverse
		PlayAnimationReverse(SwapOverlayPositionsAnim);
		bPositionNeedsSwap = false;
	}
}
