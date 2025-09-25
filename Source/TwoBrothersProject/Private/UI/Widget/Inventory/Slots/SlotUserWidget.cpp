
#include "UI/Widget/Inventory/Slots/SlotUserWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Inventory/Items/TBInventoryItem.h"
#include "UI/Widget/Inventory/Items/HoverItemUserWidget.h"
#include "UI/Widget/Inventory/Items/SlottedItemUserWidget.h"

const TMap<EGridSlotState, FLinearColor>& USlotUserWidget::GetMappedTints()
{
	static const TMap<EGridSlotState, FLinearColor> MappedTints = {
		{EGridSlotState::Default, FLinearColor(.6f,.6f,.6f,1.f)},
		{EGridSlotState::DefaultHovered, FLinearColor::White},
		{EGridSlotState::Compatible, FLinearColor(0.f, .6f, 0.f, 1.f)},
		{EGridSlotState::CompatibleHovered, FLinearColor(0.f, 1.f, 0.f, 1.f)},
		{EGridSlotState::Incompatible, FLinearColor(.6f, 0.f, 0.f, 1.f)},
		{EGridSlotState::IncompatibleHovered, FLinearColor(1.f, 0.f, 0.f, 1.f)},
		{EGridSlotState::Locked, FLinearColor::Black}
	};
	return MappedTints;
}

void USlotUserWidget::SetSlottedItem(USlottedItemUserWidget* SlottedItem)
{
	if (SizeBox_Holder->HasAnyChildren())
	{
		UE_LOG(LogTemp, Warning, TEXT("Slot already has a slotted item, ERROR!"));
		return;
	}
	
	SizeBox_Holder->AddChild(SlottedItem);
	
	USizeBoxSlot* SlottedItemSizeBoxSlot = UWidgetLayoutLibrary::SlotAsSizeBoxSlot(SlottedItem);
	SlottedItemSizeBoxSlot->SetVerticalAlignment(VAlign_Center);
	SlottedItemSizeBoxSlot->SetHorizontalAlignment(HAlign_Center);
}

void USlotUserWidget::RemoveSlottedItem()
{
	SizeBox_Holder->ClearChildren();
}

void USlotUserWidget::SetLocked()
{
	bAvailable = false;
	SlotState = EGridSlotState::Locked;
	SetTint(SlotState);
	Image_Lock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void USlotUserWidget::SetUnlocked()
{
	bAvailable = true;
	Image_Lock->RemoveFromParent();
	SlotState = EGridSlotState::Default;
	SetTint(SlotState);
}

void USlotUserWidget::SetTint(const EGridSlotState& NewSlotState)
{
	SlotState = NewSlotState;
	if (const FLinearColor* LinearColor = GetMappedTints().Find(SlotState); LinearColor)
	{
		if (Image_Slot)
		{
			Image_Slot->SetBrushTintColor(*LinearColor);
		}
	}
}

TWeakObjectPtr<UTBInventoryItem> USlotUserWidget::GetInventoryItem() const
{
	return InventoryItem;
}
 
void USlotUserWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                        UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	PreviousSlotState = SlotState;
	
	if (SlotState != EGridSlotState::Locked)
	{
		// Is currently holding item
		if (UHoverItemUserWidget* HoverItemUserWidgetPtr = Cast<UHoverItemUserWidget>(InOperation->Payload))
		{
			HoverItemUserWidgetPtr->SetHoveredOverWidget(this);
			
			/* Highlight Slot */
			// Does slot have a preference
			if (UTBInventoryItem* HoverInventoryItem = HoverItemUserWidgetPtr->GetInventoryItem(); IsValid(HoverInventoryItem))
			{
				if (HasCategoryPreference())
				{
					// Does item meet slot requirements?
					if (DoesItemMeetSlotRequirement(HoverInventoryItem))
					{
						SetTint(EGridSlotState::CompatibleHovered);
					}
					else
					{
						SetTint(EGridSlotState::IncompatibleHovered);
					}
				}
				else
				{
					SetTint(EGridSlotState::DefaultHovered);
				}

				if (HoverItemUserWidgetPtr->IsOpposingMouseBeingHeld())
				{
					RequestPlaceItemInSlot(HoverItemUserWidgetPtr, HoverInventoryItem, false);
				}
			}
		}
	}
}

void USlotUserWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UHoverItemUserWidget* HoverItemUserWidget = Cast<UHoverItemUserWidget>(InOperation->Payload))
	{
		HoverItemUserWidget->SetHoveredOverWidget(nullptr);
		OnDragUpdate(HoverItemUserWidget);
	}
	
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}

bool USlotUserWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (UHoverItemUserWidget* HoverItemWidget = Cast<UHoverItemUserWidget>(InOperation->Payload))
	{
		HoverItemWidget->SetHoveredOverWidget(nullptr);
		HoverItemWidget->OnOpposingMouseLetGo.RemoveAll(this);
		if (UTBInventoryItem* HoverInventoryItem = HoverItemWidget->GetInventoryItem(); IsValid(HoverInventoryItem))
		{
			RequestPlaceItemInSlot(HoverItemWidget, HoverInventoryItem, true);
			return true;
		}
	}
	return false;
}

void USlotUserWidget::HandleOpposingClick(UHoverItemUserWidget* HoverWidget)
{
	if (IsValid(HoverWidget))
	{
		if (UTBInventoryItem* HoverInventoryItem = HoverWidget->GetInventoryItem(); IsValid(HoverInventoryItem))
		{
			RequestPlaceItemInSlot(HoverWidget, HoverInventoryItem, false);
		}
	}
}

bool USlotUserWidget::RequestPlaceItemInSlot(UHoverItemUserWidget* InHoverItemWidget, const UTBInventoryItem* Item, bool IsDrop)
{
	if (bAvailable & DoesItemMeetSlotRequirement(Item))
	{
		if (IsDrop || (!InHoverItemWidget->IsStackable() && InHoverItemWidget->IsOpposingMouseBeingHeld()))
		{
			// Request entire placement
			OnRequestPlaceHoverItem.ExecuteIfBound(InHoverItemWidget, TileIndex, InHoverItemWidget->GetStackCount(), IsDrop);
			OnDrop();
			
			return true;
		}

		/* Opposing Mouse Click */
		// Has an item already been queued?
		if (!bIsSelected)
		{
			if (InHoverItemWidget->DidOpposingClickStartWithCtrl())
			{
				// Request Even Split
				OnRequestEvenPlaceHoverItem.ExecuteIfBound(InHoverItemWidget, TileIndex);
			}
			else
			{
				// Request single placement
				OnRequestPlaceHoverItem.ExecuteIfBound(InHoverItemWidget, TileIndex, 1, IsDrop);
			}
			bIsSelected = true;
			OnDragUpdate(InHoverItemWidget);
			return true;
		}
	}
	else
	{
		DragRejected.ExecuteIfBound(InHoverItemWidget);
		OnDrop();
	}

	return false;
}

void USlotUserWidget::OnDragUpdate(UHoverItemUserWidget* InHoverItemWidget)
{
	if (InHoverItemWidget->IsOpposingMouseBeingHeld())
	{
		InHoverItemWidget->OnOpposingMouseLetGo.AddUObject(this, &ThisClass::DeselectSlot);
	}
	else
	{
		DeselectSlot();
	}
}

void USlotUserWidget::OnDrop()
{
	SetTint(EGridSlotState::Default);
	bIsSelected = false;
}

void USlotUserWidget::DeselectSlot()
{
	SetTint(PreviousSlotState);
	bIsSelected = false;
}

void USlotUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetTint(SlotState);
}

void USlotUserWidget::SetInventoryItem(UTBInventoryItem* NewItem)
{
	InventoryItem = NewItem;
}

bool USlotUserWidget::DoesItemMeetSlotRequirement(const UTBInventoryItem* Item) const
{
	return (HasCategoryPreference()) ? MatchesCategory(Item) : true;
}

bool USlotUserWidget::MatchesCategory(const UTBInventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategoryPreference;
}

bool USlotUserWidget::HasCategoryPreference() const
{
	return !(ItemCategoryPreference.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("ItemCategories.None"))));
}