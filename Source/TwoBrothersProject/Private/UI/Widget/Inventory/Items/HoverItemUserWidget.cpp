// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Inventory/Items/HoverItemUserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
/* Keep here for the weakobjptr below*/
#include "Inventory/Items/TBInventoryItem.h"
#include "UI/Widget/Inventory/Interface/HoverItemTargetInterface.h"
#include "UI/Widget/Inventory/Slots/SlotContainerUserWidget.h"


void UHoverItemUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

	HandleOpposingMouseUp();
}

bool UHoverItemUserWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	HandleOpposingMouseUp();

	return false;
}

void UHoverItemUserWidget::HandleOpposingMouseDown(bool bIsControlDown)
{
	bOpposingMouseClickHeld = true;
	bOpposingClickStartedWithCtrl = bIsControlDown;
	StackCountOnClickStarted = StackCount;
	
	if (HoveredOverWidget.IsValid())
	{
		if (auto* ViableTarget = Cast<IHoverItemTargetInterface>(HoveredOverWidget.Get()); ViableTarget != nullptr)
		{
			ViableTarget->HandleOpposingClick(this);
		}
	}
}

// TODO: Need to call this somewhere
void UHoverItemUserWidget::HandleOpposingMouseUp()
{
	bOpposingMouseClickHeld = false;
	bOpposingClickStartedWithCtrl = false;
	StackCountOnClickStarted = 0;
	
	OnOpposingMouseLetGo.Broadcast();
}

void UHoverItemUserWidget::SetInventoryItem(UTBInventoryItem* Item)
{
	InventoryItem = Item;
}

void UHoverItemUserWidget::UpdateStackCount(int32 InStackCount)
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

void UHoverItemUserWidget::SetIsStackable(bool bStacks)
{
	bIsStackable = bStacks;
	if (!bIsStackable)
	{
		TextBlock_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHoverItemUserWidget::SetIcon(UMaterialInstance* IconMaterial, const FLinearColor& RarityColor)
{
	if (IsValid(IconMaterial))
	{
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(IconMaterial, this);
		DynamicMaterial->SetVectorParameterValue(TEXT("RarityColor"), RarityColor);
		Image_Icon->SetBrushFromMaterial(DynamicMaterial);
	}
	else
	{
		Image_Icon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHoverItemUserWidget::SetLevel(int32 Level, const FLinearColor& RarityColor)
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
		TextBlock_Level_Prefix->SetVisibility(ESlateVisibility::Collapsed);
		TextBlock_Level->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHoverItemUserWidget::SetLevelProgress(float ProgressPercentage)
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

void UHoverItemUserWidget::SetUnlocked(bool bMeetsItemRequirements)
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

void UHoverItemUserWidget::SetOwningSlotContainer(USlotContainerUserWidget* SlotContainer)
{
	OwningSlotContainer = SlotContainer;
}

FGameplayTag UHoverItemUserWidget::GetItemType() const
{
	return (IsValid(GetInventoryItem())) ? GetInventoryItem()->GetItemManifest().GetItemType()
											  : FGameplayTag::EmptyTag;
}

void UHoverItemUserWidget::SetDragInput(bool bIsLeftClickDrag)
{
	(bIsLeftClickDrag) ? LeftClickIsDrag=true : RightClickIsDrag=true;
}
