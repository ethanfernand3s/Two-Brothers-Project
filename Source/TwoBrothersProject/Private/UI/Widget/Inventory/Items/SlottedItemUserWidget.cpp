// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Inventory/Items/SlottedItemUserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
/* Keep here for the weakobjptr below*/
#include "Inventory/Items/TBInventoryItem.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/Widget/Inventory/Items/HoverItemUserWidget.h"

void USlottedItemUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
}

void USlottedItemUserWidget::NativeDestruct()
{
	ResetDetectionVariables();
}

void USlottedItemUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	SetKeyboardFocus();
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void USlottedItemUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (HasKeyboardFocus())
	{
		FSlateApplication::Get().ClearKeyboardFocus(EFocusCause::Mouse);
	}
}

FReply USlottedItemUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsDropInputPressed(InKeyEvent))
	{
		if (InKeyEvent.IsControlDown())
		{
			OnRequestDrop.ExecuteIfBound(GridIndex, StackCount);
		}
		else
		{
			OnRequestDrop.ExecuteIfBound(GridIndex, 1);
		}
	}
	
	return FReply::Handled();
}

FReply USlottedItemUserWidget::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if ( MouseEvent.IsShiftDown())
	{
		OnRequestQuickMove.ExecuteIfBound(GridIndex);
		return FReply::Handled();
	}

	RightClickHeld = IsRightClick(MouseEvent);
	LeftClickHeld  = IsLeftClick(MouseEvent);

	if (!bIsDragging && !bMouseIsHeld && (RightClickHeld || LeftClickHeld))
	{
		bMouseIsHeld = true;

		if (LeftClickHeld)
		{
			return UWidgetBlueprintLibrary::DetectDragIfPressed(
				MouseEvent, this, EKeys::LeftMouseButton
			).NativeReply;
		}
		else if (RightClickHeld)
		{
			return UWidgetBlueprintLibrary::DetectDragIfPressed(
				MouseEvent, this, EKeys::RightMouseButton
			).NativeReply;
		}
	}

	return FReply::Handled();
}

/* Only fires for the same button clicked twice, fires after button down */
FReply USlottedItemUserWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,
                                                              const FPointerEvent& InMouseEvent)
{
	if (!bIsDragging && !bMouseIsHeld && IsLeftClick(InMouseEvent) && !InMouseEvent.IsShiftDown())
	{
		bMouseIsHeld =	true;
		LeftClickCombineStart = true;
		
		LeftClickHeld = false;
		RightClickHeld = false;

		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return FReply::Handled();
}

FReply USlottedItemUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Need to make sure the button being released is the same as the one we started with
	if (RightClickHeld && IsRightClick(InMouseEvent))
	{
		bMouseIsHeld = false;
		RightClickHeld = false;
	}
	else if (LeftClickHeld && IsLeftClick(InMouseEvent))
	{
		bMouseIsHeld = false;
		LeftClickHeld = false;
	}
	else if (LeftClickCombineStart && IsLeftClick(InMouseEvent))
	{
		bMouseIsHeld = false;
		LeftClickCombineStart = false;
	}
	
	return FReply::Handled();
}

void USlottedItemUserWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                                  UDragDropOperation*& OutOperation)
{
	bIsDragging = true;
	
	// Create default operation for drag to work
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());

	// Fill in operation in inventory class
	if (LeftClickHeld)
	{
		OnRequestPickup.ExecuteIfBound(GridIndex, StackCount, false, OutOperation, true);
	}
	else if (LeftClickCombineStart)
	{
		OnRequestPickup.ExecuteIfBound(GridIndex,StackCount, true, OutOperation, true);
	}
	else if (RightClickHeld)
	{
		if (IsStackable() && StackCount > 1)
		{
			OnRequestPickup.ExecuteIfBound(GridIndex, (StackCount / 2), false, OutOperation, false);	
		}
		else
		{
			OnRequestPickup.ExecuteIfBound(GridIndex, StackCount, false, OutOperation, false);	
		}
	}

	ResetDetectionVariables();
	
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

void USlottedItemUserWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (UHoverItemUserWidget* HoverItem = Cast<UHoverItemUserWidget>(InOperation->Payload))
	{
		OnDragRejected.ExecuteIfBound(HoverItem);
	}
	
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}

void USlottedItemUserWidget::ResetDetectionVariables()
{
	bIsDragging = false;
	bMouseIsHeld = false;
	LeftClickCombineStart = false;
	RightClickHeld = false;
	LeftClickHeld = false;

	auto& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(TapTimerHandle);
}

bool USlottedItemUserWidget::IsRightClick(const FPointerEvent& MouseEvent)
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool USlottedItemUserWidget::IsLeftClick(const FPointerEvent& MouseEvent)
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}

bool USlottedItemUserWidget::IsDropInputPressed(const FKeyEvent& KeyEvent) const
{
	if (!DropAction) return false;

	const APlayerController* PlayerController = GetOwningPlayer();
	const ULocalPlayer* LocalPlayer = PlayerController ? PlayerController->GetLocalPlayer() : nullptr;
	const UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem =
		(LocalPlayer ? ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer) : nullptr);

	if (!EnhancedInputLocalPlayerSubsystem) return false;

	const FKey Pressed = KeyEvent.GetKey();
	TArray<FKey> KeysForAction = EnhancedInputLocalPlayerSubsystem->QueryKeysMappedToAction(DropAction);
	if (KeysForAction.Contains(Pressed)) return true;

	return false;
}

void USlottedItemUserWidget::SetInventoryItem(const UTBInventoryItem* Item)
{
	InventoryItem = Item;
}

void USlottedItemUserWidget::UpdateStackCount(int32 InStackCount)
{
	StackCount = InStackCount;
	if (StackCount > 0)
	{
		TextBlock_StackCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TextBlock_StackCount->SetText(FText::AsNumber(StackCount));
	}
	else
	{
		TextBlock_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USlottedItemUserWidget::SetIcon(UMaterialInstance* IconMaterial, const FLinearColor& RarityColor)
{
	if (IsValid(IconMaterial) && IsValid(Image_Icon))
	{
		// Create a dynamic material
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(IconMaterial, this);
		DynamicMaterial->SetVectorParameterValue(TEXT("RarityColor"), RarityColor);
		Image_Icon->SetBrushFromMaterial(DynamicMaterial);
	}
	else if (IsValid(Image_Icon))
	{
		Image_Icon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USlottedItemUserWidget::SetLevel(int32 Level, const FLinearColor& RarityColor)
{
	if (Level > 0)
	{
		TextBlock_Level_Prefix->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TextBlock_Level_Prefix->SetColorAndOpacity(RarityColor);
		
		TextBlock_Level->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TextBlock_Level->SetText(FText::AsNumber(Level));
		TextBlock_Level->SetColorAndOpacity(RarityColor);
	}
	else
	{
		TextBlock_Level->SetVisibility(ESlateVisibility::Collapsed);
		TextBlock_Level_Prefix->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USlottedItemUserWidget::SetLevelProgress(float ProgressPercentage)
{
	if (ProgressPercentage >= 0.f)
	{
		ProgressBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ProgressBar->SetPercent(ProgressPercentage);
	}
	else
	{
		ProgressBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}

// TODO: Fill out after deciding how to do it
void USlottedItemUserWidget::SetUnlocked(bool bMeetsItemRequirements)
{
	bIsUnlocked = bMeetsItemRequirements;

	if (!bIsUnlocked)
	{
		UMaterialInstanceDynamic* DynamicMaterial = Image_Icon->GetDynamicMaterial();
		DynamicMaterial->SetVectorParameterValue(TEXT("OverallTint"), FLinearColor::Gray);
		
		TextBlock_StackCount->SetVisibility(ESlateVisibility::Collapsed);
		TextBlock_Level->SetVisibility(ESlateVisibility::Collapsed);
		TextBlock_Level_Prefix->SetVisibility(ESlateVisibility::Collapsed);
		ProgressBar->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		UMaterialInstanceDynamic* DynamicMaterial = Image_Icon->GetDynamicMaterial();
		DynamicMaterial->SetVectorParameterValue(TEXT("OverallTint"), FLinearColor::White);
		
		if (bIsStackable) TextBlock_StackCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (bHasLevel)
		{
			TextBlock_Level->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TextBlock_Level_Prefix->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ProgressBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}
