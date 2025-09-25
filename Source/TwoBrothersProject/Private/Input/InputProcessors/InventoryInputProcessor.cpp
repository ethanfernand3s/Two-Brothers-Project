// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/InputProcessors/InventoryInputProcessor.h"

#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Widget/Inventory/Items/HoverItemUserWidget.h"

bool FInventoryInputProcessor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& InMouseEvent)
{
	if (SlateApp.IsDragDropping())
	{
		if (UDragDropOperation* Op = UWidgetBlueprintLibrary::GetDragDroppingContent())
		{
			if (UHoverItemUserWidget* ActiveHoverItem = Cast<UHoverItemUserWidget>(Op->Payload))
			{
				// Check if it was an opposing click
				if ((ActiveHoverItem->IsLeftClickDrag() && InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) ||
					(!ActiveHoverItem->IsLeftClickDrag() && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton))
				{
					ActiveHoverItem->HandleOpposingMouseDown(InMouseEvent.IsControlDown());
				}

				// As long as it's not the drag button we shouldn't stop the drag
				if (!((ActiveHoverItem->IsLeftClickDrag() && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton ||
					 (!ActiveHoverItem->IsLeftClickDrag() && InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton))))
				{
					return true;
				}
			}
		}
	}
	
	return false; // let normal flow continue
}

bool FInventoryInputProcessor::HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& InMouseEvent)
{
	if (SlateApp.IsDragDropping())
	{
		if (UDragDropOperation* Op = UWidgetBlueprintLibrary::GetDragDroppingContent())
		{
			if (UHoverItemUserWidget* ActiveHoverItem = Cast<UHoverItemUserWidget>(Op->Payload))
			{
				// Check if it was an opposing click
				if ((ActiveHoverItem->IsLeftClickDrag() && InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) ||
					(!ActiveHoverItem->IsLeftClickDrag() && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton))
				{
					ActiveHoverItem->HandleOpposingMouseUp();
				}

				// As long as it's not the drag button we shouldn't stop the drag
				if (!((ActiveHoverItem->IsLeftClickDrag() && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton ||
				     (!ActiveHoverItem->IsLeftClickDrag() && InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton))))
				{
					return true;
				}
			}
		}
	}
	
	return false; // let normal flow continue
}