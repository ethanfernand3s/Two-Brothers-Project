#include "UI/Widget/Overlay/OverlayUserWidget.h"

#include "Animation/WidgetAnimation.h"
#include "UI/Widget/Inventory/Slots/SlotContainerUserWidget.h"
#include "UI/Widget/Overlay/CharacterDetails/CharacterDetailsOverlayUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UOverlayUserWidget::SetWidgetController(UBaseWidgetController* InWidgetController)
{
	Super::SetWidgetController(InWidgetController);
	ParasiteDetailsOverlay->SetWidgetController(InWidgetController);
	AnimalDetailsOverlay->SetWidgetController(InWidgetController);

	MainAbility_SlotContainer_Parasite->SetWidgetController(InWidgetController);
	MainAbility_SlotContainer_Parasite->SetContainerOwner(true);
	DefaultAbilities_SlotContainer_Parasite->SetWidgetController(InWidgetController);
	DefaultAbilities_SlotContainer_Parasite->SetContainerOwner(true);

	MainAbility_SlotContainer_Animal->SetWidgetController(InWidgetController);
	MainAbility_SlotContainer_Animal->SetContainerOwner(false);
	DefaultAbilities_SlotContainer_Animal->SetWidgetController(InWidgetController);
	DefaultAbilities_SlotContainer_Animal->SetContainerOwner(false);
}

void UOverlayUserWidget::OnWidgetControllerSet()
{
	if ((OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController)))
	{
		SwapOverlayPositions(OverlayWidgetController->IsAnimalInhabited());
			
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

		OverlayWidgetController->AuraChanged.AddLambda(
			[this](float NewAura, bool bIsParasiteVal)
			{
				ForwardToChild<UCharacterDetailsOverlayUserWidget>(
					bIsParasiteVal, &UCharacterDetailsOverlayUserWidget::OnAuraUpdated, NewAura);
			});

		OverlayWidgetController->MaxAuraChanged.AddLambda(
			[this](float NewMaxAura, bool bIsParasiteVal)
			{
				ForwardToChild<UCharacterDetailsOverlayUserWidget>(
					bIsParasiteVal, &UCharacterDetailsOverlayUserWidget::OnMaxAuraUpdated, NewMaxAura);
			});
	}
}

void UOverlayUserWidget::OnWidgetControllerRebound(bool bIsAnimalInhabited)
{
	SwapOverlayPositions(bIsAnimalInhabited);
}

void UOverlayUserWidget::SwapOverlayPositions(bool bIsAnimalInhabited)
{
	if (bIsAnimalInhabited && !bWasAnimalInhabited)
	{
		AnimalDetailsOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		DefaultAbilities_SlotContainer_Animal->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		MainAbility_SlotContainer_Animal->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		// Play Anim forward
		PlayAnimation(SwapOverlayPositionsAnim);
	}
	else if (!bIsAnimalInhabited && bWasAnimalInhabited)
	{
		// Play Anim reverse
		PlayAnimationReverse(SwapOverlayPositionsAnim);

		float AnimTime = SwapOverlayPositionsAnim->GetEndTime() - SwapOverlayPositionsAnim->GetStartTime();
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,[this]()
		{
			if (AnimalDetailsOverlay)
			{
				AnimalDetailsOverlay->SetVisibility(ESlateVisibility::Collapsed);
				DefaultAbilities_SlotContainer_Animal->SetVisibility(ESlateVisibility::Collapsed);
				MainAbility_SlotContainer_Animal->SetVisibility(ESlateVisibility::Collapsed);
			}
		},AnimTime, false);
	}

	bWasAnimalInhabited = bIsAnimalInhabited;
}
