#include "UI/Widget/Inventory/Slots/SlotContainerUserWidget.h"

#include "TBGameplayTags.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/CanvasPanel.h"
#include "Components/UniformGridPanel.h"
#include "Inventory/Components/TBItemComponent.h"
#include "Inventory/Items/TBInventoryItem.h"
#include "Inventory/Utils/InventoryStatics.h"
#include "UI/Data/UIDataAsset.h"
#include "UI/Helpers/Utils/WidgetUtils.h"
#include "UI/Widget/Inventory/InventoryUserWidget.h"
#include "UI/Widget/Inventory/Items/HoverItemUserWidget.h"
#include "UI/Widget/Inventory/Items/ItemInfoUserWidget.h"
#include "UI/Widget/Inventory/Items/SlottedItemUserWidget.h"
#include "UI/WidgetController/BaseWidgetController.h"
#include "UI/WidgetController/InventoryWidgetController.h"

void USlotContainerUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    
    InventoryComponent = UInventoryStatics::GetInventoryComponent(GetOwningPlayer());
    InventoryComponent->OnItemAdded.AddUObject(this, &ThisClass::AddItem);
    InventoryComponent->OnItemStatusChanged.AddUObject(this, &ThisClass::UpdateItemStatus);
    InventoryComponent->OnStackChange.AddUObject(this, &ThisClass::AddStacks);
    InventoryComponent->OnInventoryMenuToggled.AddUObject(this, &ThisClass::OnInventoryMenuToggled);
}

void USlotContainerUserWidget::OnWidgetControllerSet()
{
    if(InventoryWidgetController = Cast<UInventoryWidgetController>(WidgetController))
    {
        ConstructGrid();
        
        // Bind any communications from ASC here
    }
}

void USlotContainerUserWidget::ConstructGrid()
{
    if (!Grid || !SlotClass) return;

    GridSlots.Reserve(Rows * Columns);
    Grid->SetSlotPadding(SlotPadding);
    
    for (int32 j = 0; j < Rows; ++j)
    {
        for (int32 i = 0; i < Columns; ++i)
        {
            USlotUserWidget* GridSlot = CreateWidget<USlotUserWidget>(this, SlotClass);
            GridSlot->SetItemCategoryPreference(ItemCategory);
            GridSlot->OnRequestPlaceHoverItem.BindUObject(this, &ThisClass::PutDownOnIndex);
            GridSlot->OnRequestEvenPlaceHoverItem.BindUObject(this, &ThisClass::EvenlyPutDownOnIndices);
            GridSlot->DragRejected.BindUObject(this, &ThisClass::DragRejected);
            
            // Set relative index 
            const FIntPoint TilePosition(i, j);
            GridSlot->SetTileIndex(UWidgetUtils::GetIndexFromPosition(TilePosition, Columns));
            if (HasCategoryPreference()) GridSlot->SetSlotInputTag(FTBGameplayTags::GetSlotInputTag(ItemCategory, GridSlot->GetTileIndex()));

            if (IsValid(InventoryWidgetController))
            {
                // InventoryWidgetController->TryUnlockGridSlot(ItemCategory, GridSlot->GetTileIndex());
            }
            else
            {
                GridSlot->SetLocked();
            }
            Grid->AddChildToUniformGrid(GridSlot, j, i);
            
            GridSlots.Add(GridSlot);
        }
    }
}

FSlotAvailabilityResult USlotContainerUserWidget::HasRoomForItem(UTBItemComponent* ItemComponent)
{
    return HasRoomForItem(ItemComponent->GetItemManifest());
}

FSlotAvailabilityResult USlotContainerUserWidget::HasRoomForItem(UTBInventoryItem* Item, int32 StackAmountOverride, int32 RequestedIndexStartOverride)
{
    return HasRoomForItem(Item->GetItemManifest(), StackAmountOverride, RequestedIndexStartOverride);
}

FSlotAvailabilityResult USlotContainerUserWidget::HasRoomForItem(const FTB_ItemManifest& ItemManifest, int32 StackAmountOverride, int32 RequestedIndexStartOverride)
{
    FSlotAvailabilityResult SlotAvailabilityResult; 
    SlotAvailabilityResult.PreferredContainerTag = ItemCategory;
    
    // Determine if the item is stackable. (Or if it can be used as XP for another item, MAYBE IDK YET)
    const FStackableFragment* StackableFragment = ItemManifest.GetFragmentOfType<FStackableFragment>();
    SlotAvailabilityResult.bStackable = StackableFragment != nullptr;
    
    // Determine how many stacks to add.
    const int32 MaxStackSize = StackableFragment ? StackableFragment->GetMaxStackSize() : 1;
    int32 RequestedAmountToFill = StackableFragment ? StackableFragment->GetStackCount() : 1;
    if (StackAmountOverride != -1 && SlotAvailabilityResult.bStackable)
    {
        RequestedAmountToFill = StackAmountOverride;
    }

    int32 EndIndex = GridSlots.Num() - 1;
    if (RequestedIndexStartOverride != -1)
    {
        FSlotAvailabilityResult RequestedIndexResult =
            CalculateSlotAvailability(RequestedIndexStartOverride, EndIndex, ItemManifest, SlotAvailabilityResult, RequestedAmountToFill, MaxStackSize);
        if (RequestedIndexResult.SlotAvailabilities.Num() != 0) return RequestedIndexResult;
        
        // Otherwise check the rest of the slots
        return CalculateSlotAvailability(0, RequestedIndexStartOverride, ItemManifest, RequestedIndexResult, RequestedAmountToFill, MaxStackSize);
    }
   
    return CalculateSlotAvailability(0, EndIndex, ItemManifest, SlotAvailabilityResult, RequestedAmountToFill, MaxStackSize);
}

FSlotAvailabilityResult USlotContainerUserWidget::CalculateSlotAvailability(int32 StartIndex, int32 EndIndex, const FTB_ItemManifest& ItemManifest,
        FSlotAvailabilityResult SlotAvailabilityResultTemp, int32 RequestedAmountToFill, int32 MaxStackSize)
{
    if (!GridSlots.IsValidIndex(StartIndex) || !GridSlots.IsValidIndex(EndIndex)) return FSlotAvailabilityResult();

    for (int32 i{StartIndex}; i <= EndIndex; ++i)
    {
        USlotUserWidget* GridSlot = GridSlots[i];
        // In the case the stack count is somehow 0 break loop early (Shouldn't happen)
        if (RequestedAmountToFill == 0) break;
        
        // Can the item fit here / Does slot allow the item
        if (!CheckSlotConstraints(GridSlot, ItemManifest.GetItemType(), MaxStackSize))
        {
            continue;
        }
        
        // How much to fill?
        const int32 ActualAmountToFillInSlot = DetermineFillAmountForSlot(SlotAvailabilityResultTemp.bStackable, MaxStackSize, RequestedAmountToFill, GridSlot);
        if (ActualAmountToFillInSlot == 0) continue;
        
        // Update the amount left to fill
        SlotAvailabilityResultTemp.TotalRoomToFill += ActualAmountToFillInSlot;
        // Add specific slot to fill
        SlotAvailabilityResultTemp.SlotAvailabilities.Emplace(
            FSlotAvailability{
                GridSlot->GetTileIndex(),
                SlotAvailabilityResultTemp.bStackable ? ActualAmountToFillInSlot : 0,
                HasValidItem(GridSlot)
            }
        );

        // How much is the Remainder?
        RequestedAmountToFill -= ActualAmountToFillInSlot;
        SlotAvailabilityResultTemp.Remainder = RequestedAmountToFill;

        if (RequestedAmountToFill <= 0) return SlotAvailabilityResultTemp;
    }

    return FSlotAvailabilityResult();
}

bool USlotContainerUserWidget::CheckSlotConstraints(const USlotUserWidget* GridSlot, const FGameplayTag& ItemType, const int32 MaxStackSize) const
{
    if (!GridSlot->GetAvailable()) return false; // If grid slot is locked we can't add anything
    
    // Has valid item, is it empty?
    if (!HasValidItem(GridSlot))
    {
        // If there is no valid item then we don't need to compare it with another other item, therefore we can add it
        return true;
    }
	
    // If so, is this a stackable item?
    const UTBInventoryItem* Item = GridSlot->GetInventoryItem().Get();
    if (!Item->IsStackable()) return false;
	
    // Is this item the same type as the item we're trying to add?
    if (!DoesItemTypeMatch(Item, ItemType)) return false;
	
    // If stackable, is this slot at the max stack size already?
    if (GridSlot->GetStackCount() >= MaxStackSize) return false;

    return true;
}

bool USlotContainerUserWidget::HasValidItem(const USlotUserWidget* GridSlot) 
{
    return GridSlot->GetInventoryItem().IsValid();
}

bool USlotContainerUserWidget::DoesItemTypeMatch(const UTBInventoryItem* Item, const FGameplayTag& ItemType) 
{
    return Item->GetItemManifest().GetItemType().MatchesTagExact(ItemType);
}

int32 USlotContainerUserWidget::DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize, const int32 AmountToFill, const USlotUserWidget* GridSlot)
{
    const int32 RoomInSlot = MaxStackSize - GetStackAmount(GridSlot);
    // If its stackable return a range between the requested amount to fill and the max amount the slot can currently take in.
    return bStackable ? FMath::Min(AmountToFill, RoomInSlot) : 1;
}

int32 USlotContainerUserWidget::GetStackAmount(const USlotUserWidget* GridSlot)
{
    int32 CurrentSlotStackCount = GridSlot->GetStackCount();
    return CurrentSlotStackCount;
}

void USlotContainerUserWidget::AddItem(UTBInventoryItem* Item)
{
    if(!IsValid(Item) || Item->GetPreferredSlotContainerTag() != ItemCategory || !DoesItemMeetContainerRequirement(Item)) return;
    
    FSlotAvailabilityResult AvailabilityResult = HasRoomForItem(Item);
    AddItemToIndices(AvailabilityResult, Item);
}

bool USlotContainerUserWidget::DoesItemMeetContainerRequirement(const UTBInventoryItem* Item) const
{
    return (HasCategoryPreference()) ? MatchesCategory(Item) : true;
}

void USlotContainerUserWidget::AddItemToIndices(FSlotAvailabilityResult AvailabilityResult, UTBInventoryItem* Item)
{
   for (const auto& SlotAvailability : AvailabilityResult.SlotAvailabilities)
   {
       AddItemAtIndex(Item, SlotAvailability.Index, AvailabilityResult.bStackable, SlotAvailability.AmountToFill);
       UpdateGridSlot(Item, SlotAvailability.Index, AvailabilityResult.bStackable, SlotAvailability.AmountToFill);
   }
}

void USlotContainerUserWidget::AddItemAtIndex(UTBInventoryItem* Item, const int32 Index, const bool bStackable,
                                              const int32 StackAmount)
{
    if (!IsValid(Item) || !IsValid(InventoryWidgetController))
    {
        return;
    }   

    auto& Tags = FTBGameplayTags::Get();

    const FLevelFragment* LevelFragment = GetFragment<FLevelFragment>(Item, Tags.Fragments_Level);
    
    // Create and fill out the slotted item
    USlottedItemUserWidget* NewSlottedItem = CreateSlottedItem(Item, bStackable, StackAmount, Index, LevelFragment, Tags);
    if (!IsValid(NewSlottedItem)) return;
    
    // Add to the grid at the specific index
    if (!IsValid(GridSlots[Index])) return;
    GridSlots[Index]->SetSlottedItem(NewSlottedItem);

    InventoryWidgetController->TryUnlockItem(Item, HasCategoryPreference());
    
    // Store new widget in a container
    SlottedItems.Add(Index, NewSlottedItem);
}

void USlotContainerUserWidget::UpdateGridSlot(UTBInventoryItem* Item, int32 Index, bool bStackableItem, const int32 StackAmount)
{
    if (!IsValid(Item))
    {
        return;
    }
    check(GridSlots.IsValidIndex(Index));

    USlotUserWidget* GridSlot = GridSlots[Index];
    if (bStackableItem)
    {
        GridSlot->SetStackCount(StackAmount);
    }

    GridSlot->SetInventoryItem(Item);
    GridSlot->SetTileIndex(Index);
}


USlottedItemUserWidget* USlotContainerUserWidget::CreateSlottedItem(UTBInventoryItem* Item, const bool bStackable,
                                                                    const int32 StackAmount, const int32 Index,
                                                                    const FLevelFragment* LevelFragment,
                                                                    const FTBGameplayTags& Tags)
{
    /* Material/Image Fragment and Rarity Fragment are REQUIRED !!! */
    checkf(SlottedItemClass, TEXT("Must set SlottedItemClass BP."));
    if (!IsValid(InventoryWidgetController->GetUIDataAsset()) || !IsValid(Item)) return nullptr;
    
    USlottedItemUserWidget* SlottedItem = CreateWidget<USlottedItemUserWidget>(GetOwningPlayer(), SlottedItemClass);

    // Must have an icon, background, held background, and rarity alpha all inside one material for the ability icon
    const FMaterialFragment* IconFragment = GetFragment<FMaterialFragment>(Item, Tags.Fragments_Material_Icon);
    // Rarity tag for setting the rarity alpha color and level text color
    const FRarityFragment* RarityFragment = GetFragment<FRarityFragment>(Item, Tags.Fragments_Rarity);
    if (!IconFragment || !RarityFragment) return nullptr;
    
    const FLinearColor* RarityColor = InventoryWidgetController->GetUIDataAsset()->GetRarityColor(RarityFragment->GetRarity());
    if (!RarityColor) return nullptr;
    
    SlottedItem->SetIcon(IconFragment->GetMaterial(), *RarityColor);
        
    // Level amount for setting the level text and progress
    if (LevelFragment)
    {
        SlottedItem->SetHasLevel(true);
        SlottedItem->SetLevel(LevelFragment->GetCurrentLevel(), *RarityColor);
        
        float LevelProgress = LevelFragment->GetProgressPercentage();
        SlottedItem->SetLevelProgress(LevelProgress);
    }
    
    const int32 StackUpdateAmount = bStackable ? StackAmount : 0;
    SlottedItem->UpdateStackCount(StackUpdateAmount);
    SlottedItem->SetIsStackable(bStackable);
    SlottedItem->SetInventoryItem(Item);
    SlottedItem->SetGridIndex(Index);
    SlottedItem->SetUnlocked(false);
    
    SlottedItem->OnRequestQuickMove.BindUObject(this, &ThisClass::TryQuickMove);
    SlottedItem->OnRequestItemInfo.BindUObject(this, &ThisClass::DisplayItemInfo);
    SlottedItem->OnRequestPickup.BindUObject(this, &ThisClass::RequestPickup);
    SlottedItem->OnRequestDrop.BindUObject(this, &ThisClass::DropItemFromSlottedItem);
    SlottedItem->OnDragRejected.BindUObject(this, &ThisClass::DragRejected);
    
    return SlottedItem;
}

void USlotContainerUserWidget::UpdateItemStatus(UTBInventoryItem* Item) const
{
    auto& Tags = FTBGameplayTags::Get();
    for (auto& SlottedItemPair : SlottedItems)
    {
        USlottedItemUserWidget* SlottedItem = SlottedItemPair.Value;
        if (SlottedItem->GetInventoryItem() == Item)
        {
            if (SlottedItem->IsUnlocked() && Item->GetItemStatus() == Tags.Status_Locked)
            {
                SlottedItem->SetUnlocked(false);
            }
            else if (!SlottedItem->IsUnlocked() && (Item->GetItemStatus() == Tags.Status_Unlocked ||
                                                    Item->GetItemStatus() == Tags.Status_Equipped))
            {
                SlottedItem->SetUnlocked(true);
            }

            // If we equipped an item that's an ability let the server know
            if (SlottedItem->IsUnlocked() && (Item->GetItemStatus() == Tags.Status_Equipped) && GetFragment<FAbilityFragment>(Item, Tags.Fragments_Ability))
            {
                const FGameplayTag& SlotInputTag = GridSlots[SlottedItem->GetGridIndex()]->GetSlotInputTag();
                InventoryWidgetController->HandleAbilityEquipped(Item, SlotInputTag);
            }
        }
    }
}

void USlotContainerUserWidget::AddStacks(const FSlotAvailabilityResult& Result)
{
    if (!IsValid(Result.Item.Get()) || Result.Item.Get()->GetPreferredSlotContainerTag() != ItemCategory || !DoesItemMeetContainerRequirement(Result.Item.Get())) return;

    for (const auto& Availability : Result.SlotAvailabilities)
    {
        if (Availability.bItemAtIndex)
        {
            const auto& GridSlot = GridSlots[Availability.Index];
            const auto& SlottedItem = SlottedItems.FindChecked(Availability.Index);
            int32 NewStackCount = GridSlot->GetStackCount() + Availability.AmountToFill;
            
            SlottedItem->UpdateStackCount(NewStackCount);
            GridSlot->SetStackCount(NewStackCount);
        }
        else
        {
            AddItemAtIndex(Result.Item.Get(), Availability.Index, Result.bStackable, Availability.AmountToFill);
            UpdateGridSlot(Result.Item.Get(), Availability.Index, Result.bStackable, Availability.AmountToFill);
        }
    }
}

void USlotContainerUserWidget::DisplayItemInfo(int32 GridIndex)
{
    check(GridSlots.IsValidIndex(GridIndex));
    UTBInventoryItem* ClickedInventoryItem = GridSlots[GridIndex]->GetInventoryItem().Get();
    
   if (IsValid(GetItemInfo()) && IsValid(ClickedInventoryItem))
   {
       const auto& Manifest = ClickedInventoryItem->GetItemManifest();
       Manifest.AssimilateInventoryFragments(ItemInfo);
       ItemInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
   }
}

void USlotContainerUserWidget::TryQuickMove(int32 GridIndex)
{
    check(GridSlots.IsValidIndex(GridIndex));
    UTBInventoryItem* ClickedInventoryItem = GridSlots[GridIndex]->GetInventoryItem().Get();

    UInventoryUserWidget* InventoryWidget = UInventoryStatics::GetInventoryWidget(GetOwningPlayer());
    // Must be valid, error otherwise
    check(InventoryWidget);

    // Get the container that the item is supposed to go
    USlotContainerUserWidget* DestinationContainer = InventoryWidget->GetDestinationContainer(this, ClickedInventoryItem->GetItemManifest().GetItemCategory());
    if (IsValid(DestinationContainer))
    {
        // If space is empty or item fails requirements
        const FSlotAvailabilityResult& Result = DestinationContainer->HasRoomForItem(ClickedInventoryItem);
        if (Result.SlotAvailabilities.Num() == 0 ||
            !DestinationContainer->DoesItemMeetContainerRequirement(ClickedInventoryItem)) return;

        ClickedInventoryItem->SetPreferredSlotContainerTag(Result.PreferredContainerTag);
        DestinationContainer->AddItem(ClickedInventoryItem);
        RemoveItemFromGrid(GridIndex);
    }
}

void USlotContainerUserWidget::RequestPickup(int32 GridIndex, int32 RequestedPickupAmount, bool bShouldCombine, UDragDropOperation*& DragOperation, bool bIsLeftClickDrag)
{
    check(GridSlots.IsValidIndex(GridIndex));
    
    UTBInventoryItem* ClickedInventoryItem = GridSlots[GridIndex]->GetInventoryItem().Get();
    if (!IsValid(ClickedInventoryItem)) return;

    const auto& ItemManifest = ClickedInventoryItem->GetItemManifest();
    // If should combine is true
    if (const FStackableFragment* StackableFragment = ItemManifest.GetFragmentOfType<FStackableFragment>(); (bShouldCombine && StackableFragment))
    {
        const int32 MaxStackSize = StackableFragment->GetMaxStackSize();
        
        if (MaxStackSize > RequestedPickupAmount)
        {
            const FGameplayTag& ItemTypeTag = ItemManifest.GetItemType();
            int32 TempIndex{0};
            
            while ((MaxStackSize > RequestedPickupAmount) && GridSlots.IsValidIndex(TempIndex))
            {
                int32 SlottedStackAmount = GridSlots[TempIndex]->GetStackCount();
                
                // If we are on the same item, the slots empty, or the slot has a full stack skip
                if (TempIndex == GridIndex || !HasValidItem(GridSlots[TempIndex]) || (SlottedStackAmount == MaxStackSize))
                { TempIndex++; continue; }

                UTBInventoryItem* SlottedInventoryItem = GridSlots[TempIndex]->GetInventoryItem().Get();
                if (DoesItemTypeMatch(SlottedInventoryItem, ItemTypeTag))
                {
                    int32 SpaceLeftToFill = MaxStackSize - RequestedPickupAmount;
                    int32 AmountToTake = FMath::Min(SpaceLeftToFill, SlottedStackAmount);

                    RequestedPickupAmount += AmountToTake;
                    RemoveItemAmountFromGrid(AmountToTake, TempIndex);
                }
                TempIndex++;
            }
        }
    }
    
    PickUp(ClickedInventoryItem, GridIndex, RequestedPickupAmount, DragOperation, bIsLeftClickDrag);
}

void USlotContainerUserWidget::DragRejected(UHoverItemUserWidget* ExternalHoverItem)
{
    PutHoverItemBack(ExternalHoverItem);
}

void USlotContainerUserWidget::OnInventoryMenuToggled(bool bOpen)
{
    if (!bOpen && HasHoverItemActive())
    {
        PutHoverItemBack(HoverItem);
    }
}

void USlotContainerUserWidget::PickUp(UTBInventoryItem* Item, int32 GridIndex, int32 AmountToPickup,
                                      UDragDropOperation*& InputDragOperation, bool bIsLeftClickDrag)
{
    if (!IsValid(Item))
    {
        return;
    }
    
    // Must be valid, error otherwise
    UInventoryUserWidget* InventoryWidget = UInventoryStatics::GetInventoryWidget(GetOwningPlayer());
    check(InventoryWidget);
    InventoryWidget->SetAllGridSlotsCompatibilityTints( true, Item);
    
    AssignHoverItem(Item, GridIndex, AmountToPickup, InputDragOperation, bIsLeftClickDrag);
    
    if (AmountToPickup == GridSlots[GridIndex]->GetStackCount())
    {
        RemoveItemFromGrid(GridIndex);
    }
    else
    {
        RemoveItemAmountFromGrid(AmountToPickup, GridIndex);
    }
}

void USlotContainerUserWidget::SetGridSlotsCompatibilityTints(bool bIsPickup, const UTBInventoryItem* ItemToCheck)
{
    if (bIsPickup)
    {
        if (ItemToCheck == nullptr) return;
        
        for (auto& GridSlot : GridSlots)
        {
            if (GridSlot->GetAvailable() && GridSlot->HasCategoryPreference())
            {
                // Does item meet slot requirements?
                if (GridSlot->DoesItemMeetSlotRequirement(ItemToCheck))
                {
                    GridSlot->SetTint(EGridSlotState::Compatible);
                }
                else
                {
                    GridSlot->SetTint(EGridSlotState::Incompatible);
                }
            }
        }
    }
    else
    {
        // It's a drop
        for (auto& GridSlot : GridSlots)
        {
            if (GridSlot->GetAvailable())
            {
                GridSlot->SetTint(EGridSlotState::Default);
            }
        }
    }
}

void USlotContainerUserWidget::AssignHoverItem(UTBInventoryItem* Item, int32 PreviousGridIndex, int32 AmountToPickup,
                                               UDragDropOperation*& InputDragOperation, bool bIsLeftClickDrag)
{
    if (!IsValid(Item))
    {
        return;
    }
    
    AssignHoverItem(Item,InputDragOperation, bIsLeftClickDrag);

    HoverItem->SetPreviousGridIndex(PreviousGridIndex);
    HoverItem->SetOwningSlotContainer(this);
    HoverItem->UpdateStackCount(Item->IsStackable() ? AmountToPickup : 0);
}

void USlotContainerUserWidget::AssignHoverItem(UTBInventoryItem* Item, UDragDropOperation*& InputDragOperation,
                                               bool bIsLeftClickDrag)
{
    checkf(HoverItemClass, TEXT("Must set HoverItemClass BP."));
    
    if (!IsValid(HoverItem))
    {
        HoverItem = CreateWidget<UHoverItemUserWidget>(GetOwningPlayer(), HoverItemClass);
    }
    
    auto& Tags = FTBGameplayTags::Get();

    // Must have an icon, background, held background, and rarity alpha all inside one material for the ability icon
    const FMaterialFragment* IconFragment = GetFragment<FMaterialFragment>(Item, Tags.Fragments_Material_Icon);
    // Rarity tag for setting the rarity alpha color and level text color
    const FRarityFragment* RarityFragment = GetFragment<FRarityFragment>(Item, Tags.Fragments_Rarity);
    
    if (!IsValid(InventoryWidgetController)) return;
    const FLinearColor* RarityColor = InventoryWidgetController->GetUIDataAsset()->GetRarityColor(RarityFragment->GetRarity());
    
    if (!IconFragment || !RarityFragment || !RarityColor) return;

    HoverItem->SetIcon(IconFragment->GetMaterial(), *RarityColor);
        
    // Level amount for setting the level text and the 
    if (const FLevelFragment* LevelFragment = GetFragment<FLevelFragment>(Item, Tags.Fragments_Level))
    {
        HoverItem->SetHasLevel(true);
        HoverItem->SetLevel(LevelFragment->GetCurrentLevel(), *RarityColor);
        HoverItem->SetLevelProgress(LevelFragment->GetProgressPercentage());
    }
    
    // For setting a locked tint and a lock icon if the current creature doesn't meet all the requirements
    if (const FRequirementsFragment* RequirementsFragment = GetFragment<FRequirementsFragment>(Item, Tags.Fragments_Requirements))
    {
        // TODO: Fill out using current creatures info
        // HoverItem->SetUnlocked(RequirementsFragment->MeetsItemRequirements(/* Creatures Requirements */));
    }
    
    HoverItem->SetIsStackable(Item->IsStackable());
    HoverItem->SetInventoryItem(Item);
    HoverItem->SetDragInput(bIsLeftClickDrag);
    HoverItem->DragRejected.BindUObject(this, &USlotContainerUserWidget::DragRejected);
    HoverItem->OnOpposingMouseLetGo.AddUObject(this, &USlotContainerUserWidget::HoverItemOpposingMouseLetGo);
    
    if (IsValid(InputDragOperation))
    {
        // Create Drag drop op
        InputDragOperation->Payload = HoverItem;
        InputDragOperation->DefaultDragVisual = HoverItem;
        InputDragOperation->Pivot   = EDragPivot::MouseDown;
    }
}

void USlotContainerUserWidget::HoverItemOpposingMouseLetGo()
{
    UInventoryUserWidget* InventoryWidget = UInventoryStatics::GetInventoryWidget(GetOwningPlayer());
    check(InventoryWidget);
    InventoryWidget->ClearSelectedSlots();
}

void USlotContainerUserWidget::RemoveItemAmountFromGrid(int32 AmountToRemove, int32 GridIndex)
{
    UTBInventoryItem* Item = GridSlots[GridIndex]->GetInventoryItem().Get();
    if (!IsValid(Item)) return;
    if (!Item->IsStackable()) return;
    
    int32 NewStackCount = GridSlots[GridIndex]->GetStackCount() - AmountToRemove;
    if (NewStackCount > 0)
    {
        SlottedItems.FindChecked(GridIndex)->UpdateStackCount(NewStackCount);
        GridSlots[GridIndex]->SetStackCount(NewStackCount);
    }
    else
    {
        RemoveItemFromGrid(GridIndex);
    }
}

void USlotContainerUserWidget::RemoveItemFromGrid(int32 GridIndex)
{
    GridSlots[GridIndex]->SetInventoryItem(nullptr);
    GridSlots[GridIndex]->SetStackCount(0);
    GridSlots[GridIndex]->RemoveSlottedItem();
    
    if (SlottedItems.Contains(GridIndex))
    {
        SlottedItems.Remove(GridIndex);
    }
}

void USlotContainerUserWidget::EvenlyPutDownOnIndices(UHoverItemUserWidget* ExternalHoverItem, int32 GridIndex)
{
    UInventoryUserWidget* InventoryWidget = UInventoryStatics::GetInventoryWidget(GetOwningPlayer());
    check(InventoryWidget);
    
    int32 NumOfSelectedSlots = (InventoryWidget->GetAmountOfPreviousSelectedSlots() + 1);
    int32 SplitStackAmount = ExternalHoverItem->GetStackCountOnClickStarted() / NumOfSelectedSlots;
    
    if (SplitStackAmount > 0)
    {
        int32 Remainder = ExternalHoverItem->GetStackCountOnClickStarted() % NumOfSelectedSlots;
        ExternalHoverItem->UpdateStackCount(SplitStackAmount + Remainder);
        
        PutDownOnIndex(ExternalHoverItem, GridIndex, SplitStackAmount, false);

        if (NumOfSelectedSlots > 1)
        {
            int32 PreviouslyGivenSplitStackAmount = ExternalHoverItem->GetStackCountOnClickStarted() / (NumOfSelectedSlots - 1);
            int32 AmountToRemove = PreviouslyGivenSplitStackAmount - SplitStackAmount;
            InventoryWidget->EvenlySplitPreviousSelectedSlotStackAmounts(AmountToRemove);
        }
        InventoryWidget->AddNewSelectedSlot(this, GridSlots[GridIndex]);
    }
    else
    {
        ClearHoverItem();
    }
}

void USlotContainerUserWidget::PutDownOnIndex(UHoverItemUserWidget* ExternalHoverItem, int32 GridIndex, int32 StackCount, bool bIsDrop)
{
    // Just to make sure we don't place a hidden emptied hover item when even splitting
    if (ExternalHoverItem->IsStackable() && ExternalHoverItem->GetStackCount() <= 0) ClearHoverItem();
    
    // Make sure everything is valid
    if (IsValid(ExternalHoverItem) &&
        IsValid(ExternalHoverItem->GetOwningSlotContainer()) &&
        GridSlots.IsValidIndex(GridIndex))
    {
        // If dropped on empty slot
        if (!HasValidItem(GridSlots[GridIndex]))
        {
            AddItemAtIndex(ExternalHoverItem->GetInventoryItem(), GridIndex, ExternalHoverItem->IsStackable(), StackCount);
            UpdateGridSlot(ExternalHoverItem->GetInventoryItem(), GridIndex, ExternalHoverItem->IsStackable(), StackCount);
            HandleHoverItemStackUpdate(ExternalHoverItem, StackCount);
        }
        else
        {
            if (!TryAddItemToSlottedItem(ExternalHoverItem, GridIndex, StackCount, bIsDrop))
            {
                DragRejected(ExternalHoverItem);
            }
        }
    }
}

void USlotContainerUserWidget::HandleHoverItemStackUpdate(UHoverItemUserWidget* ExternalHoverItem,
    int32 AmountTakenFromStack)
{
    int32 NewStackCount = ExternalHoverItem->GetStackCount() - AmountTakenFromStack;
    if (NewStackCount <= 0)
    {
        if (ExternalHoverItem->DidOpposingClickStartWithCtrl())
        {
            if (!ExternalHoverItem->GetOwningSlotContainer()->IsHoverItemHidden()) ExternalHoverItem->GetOwningSlotContainer()->HideHoverItem();
            ExternalHoverItem->UpdateStackCount(0);
        }
        else
        {
            ExternalHoverItem->GetOwningSlotContainer()->ClearHoverItem(); 
        }
               
    }
    else
    {
        if (ExternalHoverItem->GetOwningSlotContainer()->IsHoverItemHidden()) ExternalHoverItem->GetOwningSlotContainer()->ShowHoverItem();
        ExternalHoverItem->UpdateStackCount(NewStackCount);
    }
}

void USlotContainerUserWidget::ClearHoverItem()
{
    if (!IsValid(HoverItem)) return;

    UInventoryUserWidget* InventoryWidget = UInventoryStatics::GetInventoryWidget(GetOwningPlayer());
    check(InventoryWidget);
    InventoryWidget->SetAllGridSlotsCompatibilityTints(false);
    
    // TODO: Move all of this into a function on the hover item
    HoverItem->SetInventoryItem(nullptr);
    HoverItem->SetIsStackable(false);
    HoverItem->SetPreviousGridIndex(INDEX_NONE);
    HoverItem->UpdateStackCount(0);
    HoverItem->SetHasLevel(false);
    HoverItem->SetLevel(0, FLinearColor::White);
    HoverItem->SetLevelProgress(-1.f);
    HoverItem->SetIcon(nullptr, FLinearColor::White);
    
    HoverItem->RemoveFromParent();
    HoverItem = nullptr;

    if (FSlateApplication::Get().IsDragDropping())
    {
        FSlateApplication::Get().CancelDragDrop();
    }
}

bool USlotContainerUserWidget::IsHoverItemHidden() const
{
    return !HoverItem->IsVisible();
}

void USlotContainerUserWidget::HideHoverItem()
{
    if (!IsValid(HoverItem)) return;

    HoverItem->SetVisibility(ESlateVisibility::Collapsed);
}

void USlotContainerUserWidget::ShowHoverItem()
{
    if (!IsValid(HoverItem)) return;

    HoverItem->SetVisibility(ESlateVisibility::Visible);
}


bool USlotContainerUserWidget::TryAddItemToSlottedItem(UHoverItemUserWidget* ExternalHoverItem, int32 GridIndex, int32 RequestedAmountPlacement, bool bIsDrop)
{
    check(GridSlots.IsValidIndex(GridIndex));
    UTBInventoryItem* InventoryItemInSlot = GridSlots[GridIndex]->GetInventoryItem().Get();
    if (!IsValid(InventoryItemInSlot)) return false;
    
    if (IsSameStackable(ExternalHoverItem, InventoryItemInSlot))
    {
        const int32 ItemInSlotStackCount = GridSlots[GridIndex]->GetStackCount();
        const FStackableFragment* StackableFragment = InventoryItemInSlot->GetItemManifest().GetFragmentOfType<FStackableFragment>();
        const int32 MaxStackSize = StackableFragment->GetMaxStackSize();
        const int32 RoomInClickedSlot = MaxStackSize - ItemInSlotStackCount;
        bIsDrop = bIsDrop || (RequestedAmountPlacement == ExternalHoverItem->GetStackCount());

        // Should we swap their stack counts? (Room in the clicked slot == 0 && RequestedAmountPlacement < MaxStackSize)
        if (bIsDrop && ShouldSwapStackCounts(RoomInClickedSlot, RequestedAmountPlacement, MaxStackSize))
        {
            return SwapStackCounts(ExternalHoverItem, ItemInSlotStackCount, RequestedAmountPlacement, GridIndex);
        }
        
        // Should we consume the hover item's stacks? (Room in the clicked slot >= RequestedAmountPlacement)
        if (ShouldConsumeRequestedHoverItemAmount(RequestedAmountPlacement, RoomInClickedSlot))
        {
            return ConsumeRequestedHoverItemAmount(ExternalHoverItem, ItemInSlotStackCount, RequestedAmountPlacement, GridIndex);
        }
        
        // Should we fill in the stacks of the clicked item? (and not consume the hover item)
        if (ShouldFillInStack(RoomInClickedSlot, RequestedAmountPlacement))
        {
            return FillInStack(ExternalHoverItem, RoomInClickedSlot, GridIndex, bIsDrop);
        }
    }
    else if (IsConsumableAsXP(ExternalHoverItem, InventoryItemInSlot))
    {
        // Fill this out
    }
		
    // Swap with the hover item.
    return SwapWithHoverItem(ExternalHoverItem, InventoryItemInSlot, GridIndex);
}

bool USlotContainerUserWidget::IsSameStackable(UHoverItemUserWidget* ExternalHoverItem, const UTBInventoryItem* Item) const
{
    const bool bIsSameItem = (Item == ExternalHoverItem->GetInventoryItem());
    const bool bIsStackable = Item->IsStackable();
    return bIsSameItem && bIsStackable && ExternalHoverItem->GetItemType().MatchesTagExact(Item->GetItemManifest().GetItemType());
}

bool USlotContainerUserWidget::ShouldSwapStackCounts(const int32 RoomInClickedSlot, const int32 RequestedAmountPlacement, const int32 MaxStackSize) const
{
    return RoomInClickedSlot == 0 && RequestedAmountPlacement < MaxStackSize;
}

// This only applies to when an item is dropped - Swaps StackCounts then puts item back
bool USlotContainerUserWidget::SwapStackCounts(UHoverItemUserWidget* ExternalHoverItem, int32 ItemInSlotStackCount, int32 RequestedAmountPlacement, int32 GridIndex)
{
    USlotUserWidget* GridSlot = GridSlots[GridIndex];
    GridSlot->SetStackCount(RequestedAmountPlacement);

    USlottedItemUserWidget* ClickedSlottedItem = SlottedItems.FindChecked(GridIndex);
    ClickedSlottedItem->UpdateStackCount(RequestedAmountPlacement);

    ExternalHoverItem->UpdateStackCount(ItemInSlotStackCount);
    PutHoverItemBack(ExternalHoverItem);
    return true;
}

void USlotContainerUserWidget::PutHoverItemBack(UHoverItemUserWidget* ExternalHoverItem)
{
    if (!IsValid(ExternalHoverItem) || !IsValid(ExternalHoverItem->GetInventoryItem())) return;
    
    FSlotAvailabilityResult Result = ExternalHoverItem->GetOwningSlotContainer()
        ->HasRoomForItem(ExternalHoverItem->GetInventoryItem(), ExternalHoverItem->GetStackCount(), ExternalHoverItem->GetPreviousGridIndex());
    Result.Item = ExternalHoverItem->GetInventoryItem();
    Result.Item->SetPreferredSlotContainerTag(Result.PreferredContainerTag);
    
    ExternalHoverItem->GetOwningSlotContainer()->AddStacks(Result);
    ExternalHoverItem->GetOwningSlotContainer()->ClearHoverItem();
}

bool USlotContainerUserWidget::ShouldConsumeRequestedHoverItemAmount(const int32 RequestedAmountPlacement,
    const int32 RoomInClickedSlot) const
{
    return RoomInClickedSlot >= RequestedAmountPlacement;
}

bool USlotContainerUserWidget::ConsumeRequestedHoverItemAmount(UHoverItemUserWidget* ExternalHoverItem, const int32 ClickedStackCount, const int32 RequestedAmountPlacement,
    const int32 Index)
{
    const int32 NewClickedStackCount = ClickedStackCount + RequestedAmountPlacement;

    GridSlots[Index]->SetStackCount(NewClickedStackCount);
    SlottedItems.FindChecked(Index)->UpdateStackCount(NewClickedStackCount);
    HandleHoverItemStackUpdate(ExternalHoverItem, RequestedAmountPlacement);
    return true;
}

bool USlotContainerUserWidget::ShouldFillInStack(const int32 RoomInClickedSlot, const int32 RequestedAmountPlacement) const
{
    // Checks if requested amount to place is greater than the space available
    return RoomInClickedSlot < RequestedAmountPlacement;
}

bool USlotContainerUserWidget::FillInStack(UHoverItemUserWidget* ExternalHoverItem, const int32 FillAmount, const int32 Index, bool bIsDrop)
{
    USlotUserWidget* GridSlot = GridSlots[Index];
    const int32 NewStackCount = GridSlot->GetStackCount() + FillAmount;

    GridSlot->SetStackCount(NewStackCount);

    USlottedItemUserWidget* ClickedSlottedItem = SlottedItems.FindChecked(Index);
    ClickedSlottedItem->UpdateStackCount(NewStackCount);
    
    HandleHoverItemStackUpdate(ExternalHoverItem, FillAmount);
    if (bIsDrop) PutHoverItemBack(ExternalHoverItem);
    
    return true;
}

bool USlotContainerUserWidget::HasHoverItemActive() const
{
    return (HoverItem != nullptr);
}

UHoverItemUserWidget* USlotContainerUserWidget::GetActiveHoverItem() const
{
    return HoverItem;
}

void USlotContainerUserWidget::DropItemFromSlottedItem(int32 GridIndex, int32 AmountToDrop)
{
    UTBInventoryItem* ItemToDrop = GridSlots[GridIndex]->GetInventoryItem().Get();
    if (!IsValid(ItemToDrop)) return;

    InventoryComponent->Server_DropItem(ItemToDrop, AmountToDrop);
    RemoveItemAmountFromGrid(AmountToDrop, GridIndex);
}

void USlotContainerUserWidget::DropItemFromHoverItem(int32 AmountToDrop)
{
    if (!IsValid(HoverItem)) return;
    if (!IsValid(HoverItem->GetInventoryItem())) return;

    if (HoverItem->IsStackable() && HoverItem->GetStackCount() <= 0)
    {
        ClearHoverItem();
        return;
    }

    InventoryComponent->Server_DropItem(HoverItem->GetInventoryItem(), AmountToDrop);

    int32 NewHoverItemStackCount = HoverItem->GetStackCount() - AmountToDrop;
    if (NewHoverItemStackCount > 0)
    {
        HoverItem->UpdateStackCount(NewHoverItemStackCount);
    }
    else
    {
        ClearHoverItem();
    }
}

UItemInfoUserWidget* USlotContainerUserWidget::GetItemInfo()
{
    if (!IsValid(ItemInfo))
    {
        ItemInfo = CreateWidget<UItemInfoUserWidget>(GetOwningPlayer(), ItemInfoClass);
        if (OwningCanvasPanel.IsValid()) OwningCanvasPanel->AddChild(ItemInfo);
    }
    return ItemInfo;
}

bool USlotContainerUserWidget::IsConsumableAsXP(UHoverItemUserWidget* ExternalHoverItem, const UTBInventoryItem* Item) const 
{
    return false; // Not implemented yet //TODO: Implement
}

bool USlotContainerUserWidget::SwapWithHoverItem(UHoverItemUserWidget* ExternalHoverItem, UTBInventoryItem* Item, const int32 GridIndex)
{
    if (!IsValid(ExternalHoverItem) || !GridSlots.IsValidIndex(GridIndex)) return false;
    
    UTBInventoryItem* HoverItemInventoryItem = ExternalHoverItem->GetInventoryItem();
    int32 HoverItemStartingGridIndex = ExternalHoverItem->GetPreviousGridIndex();

    // Check if slots items meet the opposing slots requirements
    if (!GridSlots[GridIndex]->DoesItemMeetSlotRequirement(HoverItemInventoryItem) ||
        !ExternalHoverItem->GetOwningSlotContainer()->GridSlots[HoverItemStartingGridIndex]->DoesItemMeetSlotRequirement(Item))
    {
        return false;
    }
    
    // Temp Hover Item Vals
    const bool bHoverItemIsStackable = ExternalHoverItem->IsStackable();
    const int32 HoverItemStackCount = ExternalHoverItem->GetStackCount();

    // Temp Slotted Item Vals
    const FStackableFragment* StackableFragment = Item->GetItemManifest().GetFragmentOfType<FStackableFragment>();
    const bool bSlottedItemIsStackable = StackableFragment != nullptr;
    const int32 SlottedItemStackCount = GridSlots[GridIndex]->GetStackCount();
    
    // Move Slotted Item to hover item starting index
    ExternalHoverItem->GetOwningSlotContainer()->AddItemAtIndex(Item, HoverItemStartingGridIndex, bSlottedItemIsStackable, SlottedItemStackCount);
    ExternalHoverItem->GetOwningSlotContainer()->UpdateGridSlot(Item, HoverItemStartingGridIndex, bSlottedItemIsStackable, SlottedItemStackCount);

    // Remove Slotted Item that's there 
    RemoveItemFromGrid(GridIndex);

    // Put in Hover Item
    AddItemAtIndex(HoverItemInventoryItem, GridIndex, bHoverItemIsStackable, HoverItemStackCount);
    UpdateGridSlot(HoverItemInventoryItem, GridIndex, bHoverItemIsStackable, HoverItemStackCount);

    // Clear Hover Item
    ExternalHoverItem->GetOwningSlotContainer()->ClearHoverItem();
    return true;
}

void USlotContainerUserWidget::SetOwningCanvasPanel(UCanvasPanel* InOwningCanvasPanel)
{
    OwningCanvasPanel = InOwningCanvasPanel;
}

bool USlotContainerUserWidget::MatchesCategory(const UTBInventoryItem* Item) const
{
    return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}

bool USlotContainerUserWidget::HasCategoryPreference() const
{
    return !(ItemCategory.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("ItemCategories.None"))));
}