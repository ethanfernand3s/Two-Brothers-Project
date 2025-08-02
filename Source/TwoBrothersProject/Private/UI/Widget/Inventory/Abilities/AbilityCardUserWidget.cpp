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
#include "UI/WidgetController/Data/AbilityCardDataAsset.h"


void UAbilityCardUserWidget::SetCardData(const FTBAbilityInfo& InData)
{
	CardData = InData;

	UpdateAbilityIcon(InData);
	UpdateBorder(InData);
	UpdateText(InData);
	UpdateProgress(InData);
	UpdateTypes(InData.AbilityCreatureTypeTag);
	UpdateSize(InData.AbilityType);
}

/* ───────── helpers ───────── */

void UAbilityCardUserWidget::UpdateAbilityIcon(const FTBAbilityInfo& Info)
{
	if (Image_Icon && Info.Icon)
		Image_Icon->SetBrushFromTexture(Info.Icon);
}

void UAbilityCardUserWidget::UpdateBorder(const FTBAbilityInfo& Info)
{
	const UAbilityCardDataAsset* StyleAsset = InventoryWidgetController ? InventoryWidgetController->AbilityCardData : nullptr;
	if (!StyleAsset) return;

	if (!Image_Border_Regular)  return;
	Image_Border_Regular->SetVisibility(ESlateVisibility::Visible);
										
	if (const FRarityAppearanceInfo* RarityAppearanceInfo = StyleAsset->RarityAppearanceMap.Find(Info.Rarity))
	{
		// Set border as gradient or not
		if (RarityAppearanceInfo->Gradient)
		{
			if (!BorderMID) BorderMID = UMaterialInstanceDynamic::Create(RarityAppearanceInfo->Gradient, this);
			BorderMID->SetVectorParameterValue(TEXT("Tint"), RarityAppearanceInfo->Tint);
			
			if (UTexture2D* const* BorderTexturePtr = StyleAsset->AbilityBorderTypeMap.Find(Info.AbilityType))
			{
				if (*BorderTexturePtr)
				{
					BorderMID->SetTextureParameterValue(TEXT("BorderTexture"), *BorderTexturePtr);
				}
			}
			
			Image_Border_Regular->SetBrushFromMaterial(BorderMID);
		}

		// Apply rarity glow on interaction
		if (Image_BorderGlow)
		{
			if (RarityAppearanceInfo->GlowMaterial)
				Image_BorderGlow->SetBrushFromMaterial(RarityAppearanceInfo->GlowMaterial);
			Image_BorderGlow->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UAbilityCardUserWidget::UpdateText(const FTBAbilityInfo& Info)
{
	const UAbilityCardDataAsset* StyleAsset =
		InventoryWidgetController ? InventoryWidgetController->AbilityCardData : nullptr;
	if (!StyleAsset) return;

	if (const FRarityAppearanceInfo* RarityAppearanceInfo = StyleAsset->RarityAppearanceMap.Find(Info.Rarity))
	{
		if (TextBlock_LevelPrefix && TextBlock_LevelNumber)
		{
			if (!TextMID)
				TextMID = UMaterialInstanceDynamic::Create(RarityAppearanceInfo->Gradient, this);
			TextMID->SetVectorParameterValue(TEXT("Tint"), RarityAppearanceInfo->Tint);

			// Set the text
			TextBlock_LevelPrefix->SetText(FText::FromString(TEXT("LV")));
			TextBlock_LevelNumber->SetText(FText::AsNumber(Info.CurrentLevel));

			// Set the color / gradient
			if (RarityAppearanceInfo->Gradient && Info.Rarity == ERarity::Prismatic)
			{
				TextBlock_LevelPrefix->SetFontMaterial(TextMID);
				TextBlock_LevelNumber->SetFontMaterial(TextMID);
			}
			else
			{
				TextBlock_LevelNumber->SetColorAndOpacity(RarityAppearanceInfo->Tint);
				TextBlock_LevelPrefix->SetColorAndOpacity(RarityAppearanceInfo->Tint);
			}
		}
	}
}


void UAbilityCardUserWidget::UpdateProgress(const FTBAbilityInfo& Info)
{
	if (!ProgressBar || !Textblock_Progress || Info.XPRequired <= 0) return;

	const float Ratio = static_cast<float>(Info.CurrentXP) / static_cast<float>(Info.XPRequired);
	ProgressBar->SetPercent(Ratio);
	Textblock_Progress->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(Info.CurrentXP), FText::AsNumber(Info.XPRequired)));
}

void UAbilityCardUserWidget::UpdateTypes(const FGameplayTag& Tag)
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

void UAbilityCardUserWidget::UpdateSize(EAbilityType AbilityType)
{
	switch (AbilityType)
	{
		case EAbilityType::Passive:
		// if hovering over a passive slot then change size otherwise do nothing;
		case EAbilityType::Main: ;
		default: ;// Do nothing already at correct size
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
	if (FPlatformTime::Seconds() - PressedTime < 0.15) return;

	UDragDropOperation* DragOp = NewObject<UDragDropOperation>();
	DragOp->Payload = this;
	DragOp->DefaultDragVisual = this;
	OutOp = DragOp;

	OnCardDragged.Broadcast(this);
}

FReply UAbilityCardUserWidget::NativeOnMouseButtonUp(const FGeometry& Geo, const FPointerEvent& E)
{
	if (Image_BorderGlow) Image_BorderGlow->SetVisibility(ESlateVisibility::Hidden);

	if (FPlatformTime::Seconds() - PressedTime < 0.15)
	{
		OnCardTapped.Broadcast(this);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}
