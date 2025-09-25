// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntitySystem/MovieSceneEntityManager.h"
#include "UI/Widget/Composite/Composite.h"
#include "UI/Widget/Inventory/Slots/SlotUserWidget.h"
#include "SlottedItemUserWidget.generated.h"

class UHoverItemUserWidget;
class UInputAction;
class UProgressBar;
class URichTextBlock;
class USizeBox;
class UImage;
class UTextBlock;
class UTBInventoryItem;


DECLARE_DELEGATE_OneParam(FOnItemSimple, int32 /*GridIndex*/);
DECLARE_DELEGATE_TwoParams(FOnItemDropRequest, int32 /*GridIndex*/, int32 /*RequestedAmount*/);
DECLARE_DELEGATE_FiveParams(FOnItemPickupRequest, int32 /*GridIndex*/,
	int32 /*RequestedAmount*/, bool /*bShouldCombine*/,   UDragDropOperation*& /*Op*/, bool bIsLeftClickDrag);

UCLASS()
class TWOBROTHERSPROJECT_API USlottedItemUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	
#pragma region Accessors
	
	void SetIcon(UMaterialInstance* IconMaterial, const FLinearColor& RarityColor);

	void SetHasLevel (bool InHasLevel) {bHasLevel = InHasLevel;}
	void SetLevel(int32 Level, const FLinearColor& RarityColor);
	void SetLevelProgress(float ProgressPercentage);

	bool IsUnlocked() const {return bIsUnlocked; }
	void SetUnlocked(bool bMeetsItemRequirements);
	bool IsStackable() const { return bIsStackable; }
	void SetIsStackable(bool bStackable) { bIsStackable = bStackable; }
	void UpdateStackCount(int32 StackCount);

	void SetGridIndex(int32 Index) { GridIndex = Index; }
	int32 GetGridIndex() const { return GridIndex; }

	void SetInventoryItem(UTBInventoryItem* Item);
	UTBInventoryItem* GetInventoryItem() const { return InventoryItem.Get(); }

#pragma endregion Accessors

	// Triggered by Left or Right Click W/Shift
	FOnItemSimple   OnRequestQuickMove;
	// Triggered by Left or Right Click
	FOnItemSimple   OnRequestItemInfo;

	FOnItemPickupRequest   OnRequestPickup;
	
	FOnItemDropRequest   OnRequestDrop;
	
	// Request to go back to OG spot
	FOnDragRejected   OnDragRejected;
	
protected:

	/* Setup/Cleanup */
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	// Q Drops
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
	// Mouse / drag
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void   NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void   NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	/* NativeOnDropDetected handled by slot */

	// TODO: Implement Hover Events For Item Popup Comparisons - Maybe only compare when hovering over another item?

private:

	void ResetDetectionVariables();
	
	/* Input checks */
	static bool IsRightClick(const FPointerEvent& MouseEvent);
	static bool IsLeftClick(const FPointerEvent& MouseEvent);
	
	/* Can rebind so we must keep generic - Default is Q */
	bool IsDropInputPressed(const FKeyEvent& KeyEvent) const;
	
	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(AllowPrivateAccess))
	UInputAction* DropAction;
	
	/* Input checks */
	
	UPROPERTY(meta=(BindWidget))         UImage*           Image_Icon					= nullptr;
	UPROPERTY(meta=(BindWidget))         UTextBlock*	   TextBlock_Level				= nullptr;
	UPROPERTY(meta=(BindWidget))         UTextBlock*	   TextBlock_Level_Prefix		= nullptr;
	UPROPERTY(meta=(BindWidget))         UTextBlock*       TextBlock_StackCount			= nullptr;
	UPROPERTY(meta=(BindWidget))         UProgressBar*     ProgressBar					= nullptr;
	// Could add another image widget for the lock but try to implement it in the material first

	int32 GridIndex;
	TWeakObjectPtr<UTBInventoryItem> InventoryItem;
	bool bIsStackable{false};
	bool bHasLevel{false};
	bool bIsUnlocked{false};
	int32 StackCount{0};

	FTimerHandle TapTimerHandle;
	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(AllowPrivateAccess=true))
	float TapThreshold{.2f};

	bool bMouseIsHeld{false};
	bool bIsDragging{false};

	// Move what is needed to the hover widget 
	bool RightClickHeld{false};
	bool LeftClickHeld{false};
	bool LeftClickCombineStart{false};
};
