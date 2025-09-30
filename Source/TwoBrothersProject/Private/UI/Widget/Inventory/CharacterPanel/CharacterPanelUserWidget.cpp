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
        Button_SwitchCharacter->OnClicked.AddDynamic(this, &ThisClass::SwitchCharacter);
    }
    
    // Default to Parasite
    if (IsValid(WidgetSwitcher_DetailSwitcher)) WidgetSwitcher_DetailSwitcher->SetActiveWidget(ParasiteDetailsUserWidget);

    if (IsValid(AnimalDetailsUserWidget)) AnimalDetailsUserWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UCharacterPanelUserWidget::OnWidgetControllerSet()
{
	ParasiteDetailsUserWidget->SetWidgetController(WidgetController);
	AnimalDetailsUserWidget->SetWidgetController(WidgetController);
	
	if(InventoryWidgetController = Cast<UInventoryWidgetController>(WidgetController))
	{
	    if (InventoryWidgetController->IsAnimalInhabited())
	    {
	        Button_SwitchCharacter->SetIsEnabled(true);
	        AnimalDetailsUserWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	        SwitchCharacter();
	    }
	    
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

void UCharacterPanelUserWidget::SwitchCharacter()
{
    if (!IsValid(WidgetSwitcher_DetailSwitcher)) return;

    int32 CurrentIndex = WidgetSwitcher_DetailSwitcher->GetActiveWidgetIndex();

    // Toggle between 0 and 1
    int32 NewIndex = (CurrentIndex == 0) ? 1 : 0;
    WidgetSwitcher_DetailSwitcher->SetActiveWidgetIndex(NewIndex);
}
