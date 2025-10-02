#include "UI/Widget/Inventory/CharacterPanel/CharacterPanelUserWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/WidgetController/InventoryWidgetController.h"
#include "UI/Widget/Inventory/CharacterPanel/CharacterDetail/CharacterDetailsUserWidget.h"


void UCharacterPanelUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

    if (IsValid(Button_SwitchCharacter))
    {
        Button_SwitchCharacter->SetIsEnabled(false);
        Button_SwitchCharacter->OnClicked.AddDynamic(this, &ThisClass::SwitchCharacterPreview);
    }

    if (IsValid(AnimalDetailsUserWidget)) AnimalDetailsUserWidget->SetVisibility(ESlateVisibility::Hidden);

    if (IsValid(WidgetSwitcher_DetailSwitcher))
    {
        for (int32 i = 0; i < WidgetSwitcher_DetailSwitcher->GetNumWidgets(); i++)
        {
            WidgetSwitcher_DetailSwitcher->GetWidgetAtIndex(i)->ForceLayoutPrepass();
        }

        // Default to Parasite
        WidgetSwitcher_DetailSwitcher->SetActiveWidget(ParasiteDetailsUserWidget);
    }
}

void UCharacterPanelUserWidget::OnWidgetControllerSet()
{
	ParasiteDetailsUserWidget->SetWidgetController(WidgetController);
	AnimalDetailsUserWidget->SetWidgetController(WidgetController);
	
	if(InventoryWidgetController = Cast<UInventoryWidgetController>(WidgetController))
	{
	    ResolveAnimalInhabitance(InventoryWidgetController->IsAnimalInhabited());
	    
        // Attribute Delegates
        InventoryWidgetController->CurrentAndMax_AttributeInfoDelegate.AddLambda(
            [this](const FTBAttributeInfo& Current, const FTBAttributeInfo& Max, bool bIsParasiteVal)
            {
                ForwardToChild<UCharacterDetailsUserWidget>(
                    bIsParasiteVal,
                    &UCharacterDetailsUserWidget::SetHealthStat,
                    Current, Max
                );
            });

        InventoryWidgetController->Single_AttributeInfoDelegate.AddLambda(
            [this](const FTBAttributeInfo& Info, bool bIsParasiteVal)
            {
                ForwardToChild<UCharacterDetailsUserWidget>(
                    bIsParasiteVal,
                    &UCharacterDetailsUserWidget::SetSingleStat,
                    Info
                );
            });

        // Character Context Delegates
        InventoryWidgetController->OnAttributePointsChangedDelegate.AddLambda(
            [this](int32 Points, bool bIsParasiteVal)
            {
                ForwardToChild<UCharacterDetailsUserWidget>(
                    bIsParasiteVal,
                    &UCharacterDetailsUserWidget::OnAttributePointsChanged,
                    Points
                );
            });

        InventoryWidgetController->OnCharacterNameChangedDelegate.AddLambda(
            [this](const FText& NewName, bool bIsParasiteVal)
            {
                ForwardToChild<UCharacterDetailsUserWidget>(
                    bIsParasiteVal,
                    &UCharacterDetailsUserWidget::OnCharacterNameChanged,
                    NewName
                );
            });

	    InventoryWidgetController->OnCharacterIconChanged.AddLambda(
            [this](UTexture2D* NewIcon, bool bIsParasiteVal)
            {
                ForwardToChild<UCharacterDetailsUserWidget>(
                    bIsParasiteVal,
                    &UCharacterDetailsUserWidget::OnCharacterIconChanged,
                    NewIcon
                );
            });
	    
        InventoryWidgetController->OnLevelChangedDelegate.AddLambda(
            [this](int32 Level, bool bIsParasiteVal)
            {
                ForwardToChild<UCharacterDetailsUserWidget>(
                    bIsParasiteVal,
                    &UCharacterDetailsUserWidget::OnLevelChanged,
                    Level
                );
            });

        InventoryWidgetController->OnTribeDataChangedDelegate.AddLambda(
            [this](const FTribeData& TribeData, bool bIsParasiteVal)
            {
                ForwardToChild<UCharacterDetailsUserWidget>(
                    bIsParasiteVal,
                    &UCharacterDetailsUserWidget::OnTribeDataChanged,
                    TribeData
                );
            });

        InventoryWidgetController->OnGenderSetDelegate.AddLambda(
            [this](const FGameplayTag& GenderTag, bool bIsParasiteVal)
            {
                ForwardToChild<UCharacterDetailsUserWidget>(
                    bIsParasiteVal,
                    &UCharacterDetailsUserWidget::OnGenderSet,
                    GenderTag
                );
            });

        InventoryWidgetController->OnRaritySetDelegate.AddLambda(
            [this](const FGameplayTag& RarityTag, bool bIsParasiteVal)
            {
                ForwardToChild<UCharacterDetailsUserWidget>(
                    bIsParasiteVal,
                    &UCharacterDetailsUserWidget::OnRaritySet,
                    RarityTag
                );
            });

        InventoryWidgetController->OnCreatureTypesSetDelegate.AddLambda(
            [this](const FGameplayTagContainer& Types, bool bIsParasiteVal)
            {
                ForwardToChild<UCharacterDetailsUserWidget>(
                    bIsParasiteVal,
                    &UCharacterDetailsUserWidget::SetType,
                    Types
                );
            });
	}
}

void UCharacterPanelUserWidget::OnWidgetControllerRebound(bool bIsAnimalInhabited)
{
    ResolveAnimalInhabitance(bIsAnimalInhabited);
}

void UCharacterPanelUserWidget::ResolveAnimalInhabitance(bool bIsAnimalInhabited)
{
    Button_SwitchCharacter->SetIsEnabled(bIsAnimalInhabited);

    if (bIsAnimalInhabited)
    {
        // Show animal on inhabitance
        WidgetSwitcher_DetailSwitcher->SetActiveWidget(AnimalDetailsUserWidget);
        InventoryWidgetController->SetIsParasiteFocusedCharacter(false);
        AnimalDetailsUserWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        ParasiteDetailsUserWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
    }
    else
    {
        // Only Parasite details visible when not inhabited
        WidgetSwitcher_DetailSwitcher->SetActiveWidget(ParasiteDetailsUserWidget);
        InventoryWidgetController->SetIsParasiteFocusedCharacter(true);
        ParasiteDetailsUserWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        AnimalDetailsUserWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UCharacterPanelUserWidget::SwitchCharacterPreview()
{
    if (!IsValid(WidgetSwitcher_DetailSwitcher) || !IsValid(InventoryWidgetController)) return;

    int32 CurrentIndex = WidgetSwitcher_DetailSwitcher->GetActiveWidgetIndex();

    if (CurrentIndex == 0) // Is Parasite
    {
        InventoryWidgetController->SetIsParasiteFocusedCharacter(false);
        // Switch to animal details
        WidgetSwitcher_DetailSwitcher->SetActiveWidget(AnimalDetailsUserWidget);

        AnimalDetailsUserWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        ParasiteDetailsUserWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
    }
    else // Is Animal
    {
        InventoryWidgetController->SetIsParasiteFocusedCharacter(true);
        // Switch to parasite details
        WidgetSwitcher_DetailSwitcher->SetActiveWidget(ParasiteDetailsUserWidget);

        ParasiteDetailsUserWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        AnimalDetailsUserWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
    }
}