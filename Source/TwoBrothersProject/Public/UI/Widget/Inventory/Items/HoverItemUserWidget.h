#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/Inventory/Slots/SlotUserWidget.h"
#include "HoverItemUserWidget.generated.h"

class USlotContainerUserWidget;
class UProgressBar;
class URichTextBlock;
class UTBInventoryItem;

DECLARE_MULTICAST_DELEGATE(FOnOpposingMouseLetGo);
/**
 * The HoverItem is the item that will appear and follow the mouse
 * when an inventory item on the grid has been dragged.
 */
class UImage;
class UTextBlock;
UCLASS()
class TWOBROTHERSPROJECT_API UHoverItemUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	void SetIcon(UMaterialInstance* IconMaterial, const FLinearColor& RarityColor);

	void SetHasLevel (bool InHasLevel) {bHasLevel = InHasLevel;}
	void SetLevel(int32 Level, const FLinearColor& RarityColor);
	void SetLevelProgress(float ProgressPercentage);
	
	void SetUnlocked(bool bMeetsItemRequirements);
	
	void UpdateStackCount(const int32 Count);
	int32 GetStackCount() const { return StackCount; }

	int32 GetStackCountOnClickStarted() const { return StackCountOnClickStarted; }
	
	bool IsStackable() const { return bIsStackable; }
	void SetIsStackable(bool bStacks);
	
	int32 GetPreviousGridIndex() const { return PreviousGridIndex; }
	void SetPreviousGridIndex(int32 Index) { PreviousGridIndex = Index; }
	
	UTBInventoryItem* GetInventoryItem() const { return InventoryItem.Get(); }
	void SetInventoryItem(const UTBInventoryItem* Item);

	USlotContainerUserWidget* GetOwningSlotContainer() const { return OwningSlotContainer.Get(); }
	void SetOwningSlotContainer(USlotContainerUserWidget* Item);

	FGameplayTag GetItemType() const;

	void SetDragInput(bool bIsLeftClickDrag);
	bool IsLeftClickDrag() const {return LeftClickIsDrag; }

	void HandleOpposingMouseDown(bool bIsControlDown);
	void HandleOpposingMouseUp();
	bool IsOpposingMouseBeingHeld() const { return bOpposingMouseClickHeld; }
	bool DidOpposingClickStartWithCtrl() const { return bOpposingClickStartedWithCtrl; }

	void SetHoveredOverWidget(UWidget* Widget) { HoveredOverWidget = Widget; };
	
	FOnOpposingMouseLetGo OnOpposingMouseLetGo;
	FOnDragRejected DragRejected;
	
protected:

	virtual void NativeDestruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
private:

	UPROPERTY(meta=(BindWidget))         UImage*           Image_Icon					= nullptr;
	UPROPERTY(meta=(BindWidget))         UTextBlock*	   TextBlock_Level				= nullptr;
	UPROPERTY(meta=(BindWidget))         UTextBlock*	   TextBlock_Level_Prefix		= nullptr;
	UPROPERTY(meta=(BindWidget))         UTextBlock*       TextBlock_StackCount			= nullptr;
	UPROPERTY(meta=(BindWidget))         UProgressBar*     ProgressBar					= nullptr;

	int32 PreviousGridIndex;
	TWeakObjectPtr<USlotContainerUserWidget> OwningSlotContainer;
	TWeakObjectPtr<const UTBInventoryItem> InventoryItem;
	TWeakObjectPtr<UWidget> HoveredOverWidget;
	
	bool bIsStackable{false};
	bool bHasLevel{false};
	bool bIsUnlocked{false};
	
	// Current Stack Count
	int32 StackCount{0};
	int32 StackCountOnClickStarted{0};
	
	bool LeftClickIsDrag{false};
	bool RightClickIsDrag{false};
	
	bool bOpposingMouseClickHeld{false};
	bool bOpposingClickStartedWithCtrl{false};
};