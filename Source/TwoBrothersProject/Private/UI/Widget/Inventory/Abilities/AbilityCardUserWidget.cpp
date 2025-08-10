// AbilityCardUserWidget.cpp
#include "UI/Widget/Inventory/Abilities/AbilityCardUserWidget.h"
#include "AbilitySystem/Data/CreatureTypeDataAsset.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "TBGameplayTags.h"
#include "UI/WidgetController/InventoryWidgetController.h"
#include "UI/WidgetController/Data/RarityDataAsset.h"


void UAbilityCardUserWidget::SetCardData(const FTBAbilityInfo& InData)
{
	CardData = InData;
	
	UpdateCardImage(InData);
	UpdateText(InData);
	UpdateProgress(InData);
	UpdateType(InData.AbilityCreatureTypeTag);
}

/* ───────── helpers ───────── */

void UAbilityCardUserWidget::UpdateCardImage(const FTBAbilityInfo& Info)
{
	const URarityDataAsset* StyleAsset = InventoryWidgetController ? InventoryWidgetController->AbilityCardData : nullptr;
	if (!StyleAsset || !Image_Icon || Image_Border || Image_BorderGlow) return;
	
	if (const FLinearColor* ColorPtr = StyleAsset->RarityColorMap.Find(Info.Rarity))
	{
			if (Info.Icon)
			{
				// Set border as gradient or not
				if (!IconMID) IconMID = Image_Icon->GetDynamicMaterial();
				IconMID->SetTextureParameterValue(TEXT("IconTexture"),Info.Icon);
				
				Image_Border->SetBrushTintColor(*ColorPtr);					

				// Apply rarity glow on interaction
				if (Image_BorderGlow)
				{
					Image_BorderGlow->SetVisibility(ESlateVisibility::Hidden);
				}
			}
	}
}

void UAbilityCardUserWidget::UpdateText(const FTBAbilityInfo& Info)
{
	const URarityDataAsset* StyleAsset =
		InventoryWidgetController ? InventoryWidgetController->AbilityCardData : nullptr;
	if (!StyleAsset) return;

	if (const FLinearColor* ColorPtr = StyleAsset->RarityColorMap.Find(Info.Rarity))
	{
		if (TextBlock_LevelPrefix && TextBlock_LevelNumber)
		{
			// Set the text
			TextBlock_LevelPrefix->SetText(FText::FromString(TEXT("LV")));
			TextBlock_LevelNumber->SetText(FText::AsNumber(Info.CurrentLevel));

			TextBlock_LevelNumber->SetColorAndOpacity(*ColorPtr);
			TextBlock_LevelPrefix->SetColorAndOpacity(*ColorPtr);
		}
	}
}


void UAbilityCardUserWidget::UpdateProgress(const FTBAbilityInfo& Info)
{
	if (!ProgressBar || Info.XPRequired <= 0) return;

	const float Ratio = static_cast<float>(Info.CurrentXP) / static_cast<float>(Info.XPRequired);
	ProgressBar->SetPercent(Ratio);
}

void UAbilityCardUserWidget::UpdateType(const FGameplayTag& Tag)
{
	if (!InventoryWidgetController || !InventoryWidgetController->CreatureTypeData) return;

	const auto& InfoMap = InventoryWidgetController->CreatureTypeData->CreatureTypeInfoMap;
	
	ECreatureType Enum = FTBGameplayTags::Get().TagToEnum(Tag);
	if (Enum == ECreatureType::None) return;

	if (const FCreatureTypeInfo* Info = InfoMap.Find(Enum))
	{
		if (!Info->TypeImage) return;

		Image_Type->SetBrushFromTexture(Info->TypeImage, true);
		Image_Type->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

/* ───────── input handling ───────── */

FReply UAbilityCardUserWidget::NativeOnMouseButtonDown(const FGeometry& Geo, const FPointerEvent& E)
{
	if (E.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		PressedPointer = E;
		PressedTime = FPlatformTime::Seconds();
		if (Image_BorderGlow) Image_BorderGlow->SetVisibility(ESlateVisibility::Visible);
		return UWidgetBlueprintLibrary::DetectDragIfPressed(E, this, EKeys::LeftMouseButton).NativeReply;
	}
	return FReply::Unhandled();
}

void UAbilityCardUserWidget::NativeOnDragDetected(const FGeometry& Geo, const FPointerEvent& E, UDragDropOperation*& OutOp)
{
	if (FPlatformTime::Seconds() - PressedTime < 0.05) return;

	UDragDropOperation* DragOp = NewObject<UDragDropOperation>();
	DragOp->Payload = this;
	DragOp->DefaultDragVisual = this;
	OutOp = DragOp;

	OnCardDragged.Broadcast(this);
}

FReply UAbilityCardUserWidget::NativeOnMouseButtonUp(const FGeometry& Geo, const FPointerEvent& E)
{
	if (Image_BorderGlow) Image_BorderGlow->SetVisibility(ESlateVisibility::Hidden);

	if (FPlatformTime::Seconds() - PressedTime < 0.05)
	{
		OnCardTapped.Broadcast(this);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}
