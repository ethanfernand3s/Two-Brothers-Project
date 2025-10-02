#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SlotUserWidget.h"
#include "Inventory/Types/GridTypes.h"
#include "UI/Widget/BaseUserWidget.h"
#include "SlotContainerUserWidget.generated.h"

struct FCreatureTypeFragment;
struct FLevelFragment;
class UInventoryWidgetController;
struct FAbilityFragment;
struct FTBGameplayTags;
class UItemInfoUserWidget;
struct FRarityFragment;
struct FContextFragment;
struct FIconFragment;
class USlottedItemUserWidget;
struct FTB_ItemManifest;
class UTBItemComponent;
class UTBInventoryComponent;
class UCanvasPanel;
class UUniformGridPanel;
class USlotUserWidget;
class UHoverItemUserWidget;

UCLASS()
class TWOBROTHERSPROJECT_API USlotContainerUserWidget : public UBaseUserWidget
{
    GENERATED_BODY()

public:

	/* HasRoomForItem-Funnels */
	FSlotAvailabilityResult HasRoomForItem(UTBItemComponent* ItemComponent);
	FSlotAvailabilityResult HasRoomForItem(const UTBInventoryItem* Item, int32 StackAmountOverride = -1, int32 RequestedIndexStartOverride = -1);
	/* ~HasRoomForItem-Funnels~ */

	/** Returns an FSlotAvailabilityResult containing the slot info needed for adding an item. */
	FSlotAvailabilityResult HasRoomForItem(const FTB_ItemManifest& ItemManifest, int32 StackAmountOverride = -1, int32 RequestedIndexStartOverride = -1);
	FSlotAvailabilityResult CalculateSlotAvailability(int32 StartIndex, int32 EndIndex, const FTB_ItemManifest& ItemManifest,
		FSlotAvailabilityResult SlotAvailabilityResultTemp, int32 RequestedAmountToFill, int32 MaxStackSize);
	/* HasRoomForItem-Helpers */
    bool CheckSlotConstraints(const USlotUserWidget* GridSlot,
                              const FGameplayTag& ItemType,
                              const int32 MaxStackSize) const;
    static bool HasValidItem(const USlotUserWidget* GridSlot);
    static bool DoesItemTypeMatch(const UTBInventoryItem* Item, const FGameplayTag& ItemType) ;
    static int32 DetermineFillAmountForSlot(bool bStackable, int32 MaxStackSize, int32 AmountToFill,
                                     const USlotUserWidget* GridSlot);
    static int32 GetStackAmount(const USlotUserWidget* GridSlot);
	/* ~HasRoomForItem-Helpers~ */
	
	/* Add Item */
    void AddItem(const UTBInventoryItem* Item);
	bool IsMeantForContainer();
	bool DoesItemMeetContainerRequirement(const UTBInventoryItem* Item) const;
    void UpdateGridSlot(const UTBInventoryItem* Item, int32 Index, bool bStackableItem, const int32 StackAmount);
	void AddItemToIndices(FSlotAvailabilityResult AvailabilityResult, const UTBInventoryItem* Item);
	void AddItemAtIndex(const UTBInventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount);
	USlottedItemUserWidget* CreateSlottedItem(const UTBInventoryItem* Item, const bool bStackable,
                                              const int32 StackAmount, const int32 Index);
	void ResolveItemStatus(const UTBInventoryItem* Item);
	void UpdateItemStatus(const UTBInventoryItem* Item);
	void UnlockSlottedItem(USlottedItemUserWidget* SlottedItem, bool bShouldUnlock = true);
							   
	/* ~Add Item~ */

	/* Add Stacks */
	void AddStacks(const FSlotAvailabilityResult& Result);

	/* Slotted Item Callbacks */

	/** Displays item info widget. */
	void DisplayItemInfo(int32 GridIndex);
	/** Moves slotted item to opposing slot container if there is space available. */
	void TryQuickMove(int32 GridIndex);

	/* Creates hover item of the entire item. */
	void RequestPickup(int32 GridIndex, int32 RequestedPickupAmount, bool bShouldCombine, UDragDropOperation*& DragOperation, bool bIsLeftClickDrag);

	/* Removes the hover item and brings the slotted item back to its original slot. */
	void DragRejected(UHoverItemUserWidget* ExternalHoverItem);
	
	/* ~Slotted Item Callbacks~ */

	/* Inventory Callbacks */
	void OnInventoryMenuToggled(bool bOpen);
	
	/* Hover Item */
	void PickUp(const UTBInventoryItem* Item, int32 GridIndex, int32 AmountToPickup,
		UDragDropOperation*& InputDragOperation, bool bIsLeftClickDrag);
	void SetGridSlotsCompatibilityTints(bool bIsPickup, const UTBInventoryItem* ItemToCheck = nullptr);
	void AssignHoverItem(const UTBInventoryItem* Item, int32 PreviousGridIndex, int32 AmountToPickup,
		UDragDropOperation*& InputDragOperation, bool bIsLeftClickDrag);
	void AssignHoverItem(const UTBInventoryItem* Item, UDragDropOperation*& InputDragOperation, bool bIsLeftClickDrag);
	void HoverItemOpposingMouseLetGo();
	void RemoveItemAmountFromGrid(int32 AmountToRemove, int32 GridIndex);
	void RemoveItemFromGrid(int32 GridIndex);
	void EvenlyPutDownOnIndices(UHoverItemUserWidget* ExternalHoverItem, int32 GridIndex);
	void PutDownOnIndex(UHoverItemUserWidget* ExternalHoverItem, int32 GridIndex, int32 StackCount, bool bIsDrop);
	void HandleHoverItemStackUpdate(UHoverItemUserWidget* ExternalHoverItem, int32 AmountTakenFromStack);
	bool TryAddItemToSlottedItem(UHoverItemUserWidget* ExternalHoverItem, int32 GridIndex, int32 StackCount, bool bIsDrop);
	void ClearHoverItem();
	bool IsHoverItemHidden() const;
	void HideHoverItem();
	void ShowHoverItem();
	bool IsSameStackable(UHoverItemUserWidget* ExternalHoverItem, const UTBInventoryItem* Item) const;
	bool IsConsumableAsXP(UHoverItemUserWidget* ExternalHoverItem, const UTBInventoryItem* Item) const;
	bool SwapWithHoverItem(UHoverItemUserWidget* ExternalHoverItem, const UTBInventoryItem* Item, const int32 GridIndex);
	bool ShouldSwapStackCounts(int32 RoomInClickedSlot, int32 HoveredStackCount, int32 MaxStackSize) const;
	bool SwapStackCounts(UHoverItemUserWidget* ExternalHoverItem, int32 ItemInSlotStackCount, int32 HoveredStackCount, int32 GridIndex);
	void PutHoverItemBack(UHoverItemUserWidget* ExternalHoverItem);
	bool ShouldConsumeRequestedHoverItemAmount(const int32 HoveredStackCount, const int32 RoomInClickedSlot) const;
	bool ConsumeRequestedHoverItemAmount(UHoverItemUserWidget* ExternalHoverItem, const int32 ClickedStackCount, const int32 HoveredStackCount, const int32 Index);
	bool ShouldFillInStack(const int32 RoomInClickedSlot, const int32 HoveredStackCount) const;
	bool FillInStack(UHoverItemUserWidget* ExternalHoverItem, const int32 FillAmount, const int32 Index,  bool bIsDrop);
	bool HasHoverItemActive() const;
	UHoverItemUserWidget* GetActiveHoverItem() const;
    /* ~Hover Item~ */

	/* Drop Item */
	void DropItemFromSlottedItem(int32 GridIndex, int32 AmountToDrop);
	void DropItemFromHoverItem(int32 AmountToDrop);
	
	/* Item Info */
	UItemInfoUserWidget* GetItemInfo();
	/* ~Item Info~ */

	/* Cached Setters */
	void SetOwningCanvasPanel(UCanvasPanel* InOwningCanvasPanel);
	void SetContainerOwner(bool InbIsParasiteContainerOwner) { bIsParasiteContainerOwner = InbIsParasiteContainerOwner;}
	
	/* Helpers */
	bool HasCategoryPreference() const;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void OnWidgetControllerSet() override;
	
private:

	void ConstructGrid();

	/* Item Category*/
	
	/* This function only matters if this slot container is used in the equipable slots */
	bool MatchesCategory(const UTBInventoryItem* Item) const;
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", GameplayTagFilter="ItemCategories"),  Category = "Inventory")
    FGameplayTag ItemCategory;

	/* Layout */
    UPROPERTY(EditAnywhere, Category="Inventory|Slots|Layout", meta=(ClampMin="1"))
    int32 Rows = 2;
    UPROPERTY(EditAnywhere, Category="Inventory|Slots|Layout", meta=(ClampMin="1"))
    int32 Columns = 4;
    UPROPERTY(EditAnywhere, Category="Inventory|Slots|Layout")
    float SlotPadding = 5.f;

	/* Grid Slots*/
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* Grid = nullptr;
	UPROPERTY()
	TArray<TObjectPtr<USlotUserWidget>> GridSlots;
	UPROPERTY(EditAnywhere, Category="Inventory|Slots|Class")
	TSubclassOf<USlotUserWidget> SlotClass;

	/* Slotted Item */
	UPROPERTY()
	TMap<int32, TObjectPtr<USlottedItemUserWidget>> SlottedItems;
	UPROPERTY(EditAnywhere, Category="Inventory|SlottedItem|Class")
	TSubclassOf<USlottedItemUserWidget> SlottedItemClass;

	/* Hovered Item */
    UPROPERTY()
	TObjectPtr<UHoverItemUserWidget> HoverItem;
    UPROPERTY(EditAnywhere, Category = "Inventory")
    TSubclassOf<UHoverItemUserWidget> HoverItemClass;

	/* Item Info */
	UPROPERTY()
	TObjectPtr<UItemInfoUserWidget> ItemInfo;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UItemInfoUserWidget> ItemInfoClass;

	/* Data */
	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
	
    /* Cached Vars */
	TWeakObjectPtr<UTBInventoryComponent> InventoryComponent;
    TWeakObjectPtr<UCanvasPanel> OwningCanvasPanel;
	bool bIsParasiteContainerOwner{false};
};
