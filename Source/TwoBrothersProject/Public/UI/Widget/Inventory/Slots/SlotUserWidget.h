#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/Widget/BaseUserWidget.h"
#include "UI/Widget/Inventory/Interface/HoverItemTargetInterface.h"
#include "SlotUserWidget.generated.h"

class UHoverItemUserWidget;
class USlottedItemUserWidget;
class UOverlay;
class UTBInventoryItem;
class UImage;
class USizeBox;
class UButton;
class UUserWidget;
class USlotContainerUserWidget;

UENUM(BlueprintType)
enum class EGridSlotState : uint8
{
	// Used for slots without requirements
	
	Default,
	DefaultHovered,
	
	//~Used for slots without requirements

	// Used for slots that have requirements
	
	Compatible,
	CompatibleHovered,
	Incompatible,
	IncompatibleHovered,

	//~Used for slots that have requirements

	// Used in the case where the slot hasn't been unlocked
	Locked
};

DECLARE_DELEGATE_FourParams(FOnRequestPlaceHoverItem, UHoverItemUserWidget* /* HoverItem */,  int32 /*GridIndex*/, int32 /*StackCount*/, bool /*bIsDrop*/);
DECLARE_DELEGATE_TwoParams(FOnRequestEvenPlaceHoverItem, UHoverItemUserWidget* /* HoverItem */,  int32 /*GridIndex*/);
DECLARE_DELEGATE_OneParam(FOnDragRejected, UHoverItemUserWidget* /* HoverItem */);
UCLASS()
class TWOBROTHERSPROJECT_API USlotUserWidget : public UBaseUserWidget, public IHoverItemTargetInterface
{
	GENERATED_BODY()

public:
	
	/* For holding the item in the UI */
	void SetSlottedItem(USlottedItemUserWidget* SlottedItem);
	/* For removing the held item in the UI */
	void RemoveSlottedItem();
	
	void SetLocked();
	void SetUnlocked();
	void SetTint(const EGridSlotState& NewSlotState);
	
	void SetTileIndex(const int32 Index) { TileIndex = Index; }
	int32 GetTileIndex() const { return TileIndex; }

	void SetAvailable(bool InbAvailable) { bAvailable = InbAvailable; }
	bool GetAvailable() const { return bAvailable; }

	/* For holding the item ptr */
	void SetInventoryItem(UTBInventoryItem* Item);
	
	/* For getting the item ptr */
	TWeakObjectPtr<UTBInventoryItem> GetInventoryItem() const;

	void SetStackCount(int32 InStackCount) { StackCount = InStackCount; }
	int32 GetStackCount() const { return StackCount; }

	void SetItemCategoryPreference(FGameplayTag ItemPreferenceTag) { ItemCategoryPreference = ItemPreferenceTag; }
	bool DoesItemMeetSlotRequirement(const UTBInventoryItem* Item) const;
	bool MatchesCategory(const UTBInventoryItem* Item) const;
	bool HasCategoryPreference() const;

	void SetSlotInputTag(const FGameplayTag& NewSlotInputTag) { SlotInputTag = NewSlotInputTag; }
	FGameplayTag GetSlotInputTag() const { return SlotInputTag; }
	
	FOnRequestPlaceHoverItem OnRequestPlaceHoverItem;
	FOnRequestEvenPlaceHoverItem OnRequestEvenPlaceHoverItem;
	FOnDragRejected DragRejected;
	
	UFUNCTION()
	void DeselectSlot();
	
	virtual void HandleOpposingClick(UHoverItemUserWidget* HoverWidget) override;
	
protected:
															/* Drag target */
	virtual void NativeConstruct() override;
	
	// Changing Tint & if opposing input is held Spreading out items single and if ctrl is on first placement then even spread
	virtual void   NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void   NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// Swap, Place, Combine, or reject
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:

	bool RequestPlaceItemInSlot(UHoverItemUserWidget* InHoverItemWidget, const UTBInventoryItem* Item, bool IsDrop);
	// Deselect All Highlighted Slots & Reset Ptr
	void OnDragUpdate(UHoverItemUserWidget* InHoverItemWidget);
	void OnDrop();
	
	EGridSlotState PreviousSlotState = EGridSlotState::Default;
	EGridSlotState SlotState = EGridSlotState::Default;
	int32 StackCount{0};
	bool bAvailable{true};
	bool bIsSelected{false};
	TWeakObjectPtr<UTBInventoryItem> InventoryItem;
	FGameplayTag ItemCategoryPreference{};
	FGameplayTag SlotInputTag{};
	/* This is the same as the upper left index since the slots are only 1x1 */
	int32 TileIndex{INDEX_NONE};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox>  SizeBox_Holder;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage>  Image_Lock;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage>  Image_Slot;
	
	static const TMap<EGridSlotState, FLinearColor>& GetMappedTints();
};
