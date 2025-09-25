// InventoryUserWidget.cpp

#include "UI/Widget/Inventory/InventoryUserWidget.h"

#include "TBGameplayTags.h"
#include "Blueprint/DragDropOperation.h"
#include "Inventory/Utils/InventoryStatics.h"
#include "UI/Widget/Inventory/InventoryBackgroundUserWidget.h"
#include "UI/Widget/Inventory/CharacterPanel/CharacterPanelUserWidget.h"
#include "UI/Widget/Inventory/Items/HoverItemUserWidget.h"
#include "UI/Widget/Inventory/Slots/SlotPanelUserWidget.h"

void UInventoryUserWidget::SetWidgetController(UBaseWidgetController* InWidgetController)
{
    if (IsValid(CharacterPanel)) { CharacterPanel->SetWidgetController(InWidgetController); }
    if (IsValid(InventoryPanel)) { InventoryPanel->SetWidgetController(InWidgetController); }
    Super::SetWidgetController(InWidgetController);
}

FSlotAvailabilityResult UInventoryUserWidget::HasRoomForItem(UTBItemComponent* ItemComponent) const
{
    const FGameplayTag ItemCategory(UInventoryStatics::GetItemCategoryFromItemComp(ItemComponent));
    if (const TWeakObjectPtr<USlotContainerUserWidget>* SlotContainerPtr = SlotContainers.Find(ItemCategory))
    {
        if (SlotContainerPtr->IsValid())
        {
            if (USlotContainerUserWidget* Container = SlotContainerPtr->Get())
            {
                const FSlotAvailabilityResult& PreferredContainerResult = Container->HasRoomForItem(ItemComponent);
                if (PreferredContainerResult.SlotAvailabilities.Num() > 0) return PreferredContainerResult;

                // Otherwise desired space isn't available add to inventory instead (if can)
                const FSlotAvailabilityResult& InventoryContainerResult = InventoryPanel->GetSlotContainer()->HasRoomForItem(ItemComponent);
                return InventoryContainerResult;
            }
        }
    }
    else if (ItemCategory.MatchesTag(FGameplayTag::RequestGameplayTag("ItemCategories")))
    {
        /* Note: Inventory can take any ItemCategory tag as long as its of ItemCategories */
        const FSlotAvailabilityResult& InventoryContainerResult = InventoryPanel->GetSlotContainer()->HasRoomForItem(ItemComponent);
        return InventoryContainerResult;
    }
    
    // Doesn't have valid item category
    UE_LOG(LogTemp, Warning, TEXT("Item category is invalid."))
    return FSlotAvailabilityResult();
}

void UInventoryUserWidget::NativeOnInitialized()
{ 
    Super::NativeOnInitialized();

    const auto& Tags = FTBGameplayTags::Get();

    /* Note: Inventory can take any ItemCategory tag as long as its of ItemCategories */
    SlotContainers.Add({Tags.ItemCategories_None, InventoryPanel->GetSlotContainer()});
    SlotContainers.Add({Tags.ItemCategories_Abilities_Main, CharacterPanel->GetMainAbilitySlotContainer()});
    SlotContainers.Add({Tags.ItemCategories_Abilities_Default, CharacterPanel->GetDefaultAbilitySlotContainer()});
    SlotContainers.Add({Tags.ItemCategories_Abilities_Passive, CharacterPanel->GetPassiveAbilitySlotContainer()});

    for (auto& SlotContainerPair : SlotContainers)
    {
        SlotContainerPair.Value->SetOwningCanvasPanel(CanvasPanel);
        USlotContainerUserWidget* const SlotContainer = SlotContainerPair.Value.Get();
        
        DimmedBackground->OnHoverItemDrop.AddLambda([SlotContainer](int32 AmountToDrop)
        {
            if (SlotContainer->HasHoverItemActive())
            {
                SlotContainer->DropItemFromHoverItem(AmountToDrop);
            }
        });
    }
}

bool UInventoryUserWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    // Not Handled by children therefore a fail.
    if (UHoverItemUserWidget* HoverItem = Cast<UHoverItemUserWidget>(InOperation->Payload))
    {
        HoverItem->GetOwningSlotContainer()->DragRejected(HoverItem);
        return true; 
    }

    return false;
}

void UInventoryUserWidget::NativeDestruct()
{
    Super::NativeDestruct();
    ClearSelectedSlots();
}

USlotContainerUserWidget* UInventoryUserWidget::GetDestinationContainer(const USlotContainerUserWidget* OriginSlotContainer, const FGameplayTag& ItemCategory) const
{
    if (!IsValid(OriginSlotContainer) || !ItemCategory.IsValid()) return nullptr;
	
    if (OriginSlotContainer->HasCategoryPreference())  return InventoryPanel->GetSlotContainer();
    
    if (const TWeakObjectPtr<USlotContainerUserWidget>* SlotContainerPtr = SlotContainers.Find(ItemCategory))
    {
        if (SlotContainerPtr->IsValid())
        {
            if (USlotContainerUserWidget* Container = SlotContainerPtr->Get())
            {
                return Container;
            }
        }
    }
    
    return nullptr;
}

void UInventoryUserWidget::SetAllGridSlotsCompatibilityTints(bool bIsPickup, const UTBInventoryItem* ItemToCheck)
{
    for (auto& SlotContainerPair : SlotContainers)
    {
        SlotContainerPair.Value->SetGridSlotsCompatibilityTints(bIsPickup, ItemToCheck);
    }
}

void UInventoryUserWidget::AddNewSelectedSlot(USlotContainerUserWidget* OwningSlotContainer, USlotUserWidget* NewestSlot)
{
    SelectedGridSlots.Add({OwningSlotContainer, NewestSlot});
}

void UInventoryUserWidget::EvenlySplitPreviousSelectedSlotStackAmounts(int32 AmountToRemove)
{    
    /* Update any selected items */
    for (auto& SelectedSlotPair : SelectedGridSlots)
    {
        USlotContainerUserWidget* OwningSlotContainer = SelectedSlotPair.Key.Get();
        USlotUserWidget* SelectedSlot = SelectedSlotPair.Value.Get();
        if (IsValid(OwningSlotContainer) && IsValid(SelectedSlot))
        {
            if (AmountToRemove > 0) OwningSlotContainer->RemoveItemAmountFromGrid(AmountToRemove, SelectedSlot->GetTileIndex());
        }
    }
}

int32 UInventoryUserWidget::GetAmountOfPreviousSelectedSlots()
{
    return SelectedGridSlots.Num();
}

void UInventoryUserWidget::ClearSelectedSlots()
{
    SelectedGridSlots.Reset();
}
