// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Inventory/InventoryBackgroundUserWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Widget/Inventory/Items/HoverItemUserWidget.h"

void UInventoryBackgroundUserWidget::HandleOpposingClick(UHoverItemUserWidget* HoverItemUserWidget)
{
	if (IsValid(HoverItemUserWidget))
	{
		if (HoverItemUserWidget->DidOpposingClickStartWithCtrl())
		{
			OnHoverItemDrop.Broadcast((HoverItemUserWidget->IsStackable() && HoverItemUserWidget->GetStackCount() > 1) ?
				(HoverItemUserWidget->GetStackCount() / 2) : 1);
		}

		OnHoverItemDrop.Broadcast(1);
	}
}

bool UInventoryBackgroundUserWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (UHoverItemUserWidget* HoverItemUserWidget = Cast<UHoverItemUserWidget>(InOperation->Payload))
	{
		HoverItemUserWidget->SetHoveredOverWidget(nullptr);
		OnHoverItemDrop.Broadcast(HoverItemUserWidget->GetStackCount());
		return true;
	}
	
	return false;
}

void UInventoryBackgroundUserWidget::NativeOnDragEnter(const FGeometry& InGeometry,
	const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UHoverItemUserWidget* HoverItemUserWidget = Cast<UHoverItemUserWidget>(InOperation->Payload))
	{
		HoverItemUserWidget->SetHoveredOverWidget(this);
	}
	
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
}

void UInventoryBackgroundUserWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (UHoverItemUserWidget* HoverItemUserWidget = Cast<UHoverItemUserWidget>(InOperation->Payload))
	{
		HoverItemUserWidget->SetHoveredOverWidget(nullptr);
	}
	
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}
