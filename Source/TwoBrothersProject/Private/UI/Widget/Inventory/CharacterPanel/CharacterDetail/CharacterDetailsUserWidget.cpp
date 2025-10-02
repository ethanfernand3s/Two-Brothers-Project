// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widget/Inventory/CharacterPanel/CharacterDetail/CharacterDetailsUserWidget.h"

#include "Kismet/GameplayStatics.h"
#include "TBGameplayTags.h"
#include "AbilitySystem/Data/CharacterCombatValues.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/Data/TribeData.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "UI/Data/UIDataAsset.h"
#include "UI/Widget/Inventory/CharacterPanel/Stats/StatButton.h"
#include "UI/Widget/Inventory/CharacterPanel/Stats/StatsRadarUserWidget.h"
#include "UI/Widget/Inventory/Slots/SlotContainerUserWidget.h"
#include "UI/Widget/Inventory/Slots/SlotPanelUserWidget.h"


#include "UI/WidgetController/InventoryWidgetController.h"

void UCharacterDetailsUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UCharacterDetailsUserWidget::OnWidgetControllerSet()
{
	InventoryWidgetController = Cast<UInventoryWidgetController>(WidgetController);
	
	if (IsValid(Button_EquippablePassiveAbilitiesContainerToggle))
	{
		Button_EquippablePassiveAbilitiesContainerToggle->OnClicked.AddDynamic(this, &UCharacterDetailsUserWidget::ToggleDropdown);
	}

	SetupStatButtons();
	
	if (IsValid(PassivePanel)) PassivePanel->SetWidgetController(WidgetController);
	if (IsValid(MainAbility_SlotContainer)) MainAbility_SlotContainer->SetWidgetController(WidgetController);
	if (IsValid(DefaultAbilities_SlotContainer)) DefaultAbilities_SlotContainer->SetWidgetController(WidgetController);
}

void UCharacterDetailsUserWidget::SetupStatButtons()
{
	// Map stats for quick lookup
	const auto Tags = FTBGameplayTags::Get();
	MappedStatBindings.Add(Tags.Attributes_MaxHealth,       {StatButton_Health,       0});
	MappedStatBindings.Add(Tags.Attributes_Strength,     {StatButton_Strength,     1});
	MappedStatBindings.Add(Tags.Attributes_Defense,      {StatButton_Defense,      2});
	MappedStatBindings.Add(Tags.Attributes_Speed,        {StatButton_Speed,        3});
	MappedStatBindings.Add(Tags.Attributes_AuraDefense,  {StatButton_AuraDefense,  4});
	MappedStatBindings.Add(Tags.Attributes_AuraStrength, {StatButton_AuraStrength, 5});

	for (const auto& StatButtonPair : MappedStatBindings)
	{
		// On Clicked
		if (IsValid(StatButtonPair.Value.StatButton))
		{
			StatButtonPair.Value.StatButton->OnStatButtonClicked.BindUObject(this, &UCharacterDetailsUserWidget::OnStatButtonPressed);
		}
	}
}

void UCharacterDetailsUserWidget::SetSingleStat(const FTBAttributeInfo& AttributeInfo)
{
	if (FStatUIBinding* Binding = MappedStatBindings.Find(AttributeInfo.AttributeTag))
	{
		const FText StatValueText = FText::AsNumber(AttributeInfo.AttributeValue);
		if (Binding->StatButton)
		{
			Binding->StatButton->SetStatText(StatValueText);
		}

		if ((IsValid(StatsRadar) && Binding->RadarIndex != INDEX_NONE))
		{
			const float Normalized = FMath::Clamp(AttributeInfo.AttributeValue / FCharacterCombatValues::MaxValueAmount, 0.f, 1.f);
			StatsRadar->UpdateSingleRadarStat(Binding->RadarIndex, Normalized);
		}
		
		PlayStatIncreasedEffects();
	}
}

void UCharacterDetailsUserWidget::SetHealthStat(const FTBAttributeInfo& CurrentAttributeInfo, const FTBAttributeInfo& MaxAttributeInfo)
{
	const float NewCurrentHealth{CurrentAttributeInfo.AttributeValue};
	const float NewMaxHealth{MaxAttributeInfo.AttributeValue}; 

	if (NewMaxHealth != 0)
	{
		FNumberFormattingOptions Format;
		Format.MinimumFractionalDigits = 1;
		Format.MaximumFractionalDigits = 1;

		FText FormattedAttributeValue = FText::Format(
			NSLOCTEXT("AttributeUI", "AttributeFormat", "{0} / {1}"),
			FText::AsNumber(NewCurrentHealth, &Format),
			FText::AsNumber(NewMaxHealth, &Format)
		);
		
		if (IsValid(StatButton_Health)) StatButton_Health->SetStatText(FormattedAttributeValue);

		// Refresh Widget only if Max Health was changed
		if (PreviousMaxHealth != NewMaxHealth)
		{
			PreviousMaxHealth = NewMaxHealth;
			if (IsValid(StatsRadar))
			{
				const float Normalized = FMath::Clamp(NewMaxHealth / FCharacterCombatValues::MaxValueAmount, 0.f, 1.f);
				StatsRadar->UpdateSingleRadarStat(0, Normalized);
			}
			PlayStatIncreasedEffects();
		}
	}
}

void UCharacterDetailsUserWidget::SetType(const FGameplayTagContainer& CreatureTypeTags)
{
	if (!IsValid(InventoryWidgetController) || !IsValid(InventoryWidgetController->GetUIDataAsset())) return;
	
	for (const auto& Tag : CreatureTypeTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("CreatureTypes"))))
		{
			if (const FCreatureTypeImages* CreatureTypeImages = InventoryWidgetController->GetUIDataAsset()->GetCreatureTypeImages(Tag))
			{
				if (!CreatureTypeImages->TypeImage_64px) return;

				// Prevent duplicates
				for (UWidget* Child : VerticalBox_Types->GetAllChildren())
				{
					if (const UImage* Img = Cast<UImage>(Child))
					{
						if (Img->GetBrush().GetResourceObject() == CreatureTypeImages->TypeImage_64px)
							return;
					}
				}

				// Cap at two icons
				if (VerticalBox_Types->GetChildrenCount() >= 2) return;

				UImage* NewImg = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
				NewImg->SetBrushFromTexture(CreatureTypeImages->TypeImage_64px, true);
				NewImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

				if (UVerticalBoxSlot* VertSlot = VerticalBox_Types->AddChildToVerticalBox(NewImg))
				{
					VertSlot->SetHorizontalAlignment(HAlign_Center);
					VertSlot->SetPadding(FMargin(0.f, 5.f));
				}
			}
		}
	}
}

void UCharacterDetailsUserWidget::OnGenderSet(const FGameplayTag& InGenderTag)
{
	if (!IsValid(InventoryWidgetController) || !IsValid(InventoryWidgetController->GetUIDataAsset())) return;
	
	if (UTexture2D* GenderImage = InventoryWidgetController->GetUIDataAsset()->GetGenderTexture(InGenderTag))
	{
		Image_Gender->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Image_Gender->SetBrushFromTexture(GenderImage);
	}
}

void UCharacterDetailsUserWidget::OnRaritySet(const FGameplayTag& InRarityTag)
{
	if (!IsValid(InventoryWidgetController) || !IsValid(InventoryWidgetController->GetUIDataAsset())) return;
	
	if (const FLinearColor* RarityColor =InventoryWidgetController->GetUIDataAsset()->GetRarityColor(InRarityTag))
	{
		TextBlock_CharacterName->SetColorAndOpacity(*RarityColor);
	}
}

void UCharacterDetailsUserWidget::OnAttributePointsChanged(int NewAttributePoints)
{
	if (!IsValid(TextBlock_AttributePoints)) return;
	
	TextBlock_AttributePoints->SetText(FText::AsNumber(NewAttributePoints));
	
	if (NewAttributePoints == 0)
	{
		UpdateAllButtons(false);
	}
	else if (NewAttributePoints == 1)
	{
		UpdateAllButtons(true);
	}
}

void UCharacterDetailsUserWidget::UpdateAllButtons(bool bShouldEnable)
{
	for (const auto& StatButtonPair : MappedStatBindings)
	{
		if (IsValid(StatButtonPair.Value.StatButton))
		{
			StatButtonPair.Value.StatButton->SetIsEnabled(bShouldEnable);
		}
	}
}

void UCharacterDetailsUserWidget::OnCharacterNameChanged(const FText& NewCharacterName)
{
	if (!IsValid(TextBlock_CharacterName)) return;
	TextBlock_CharacterName->SetText(NewCharacterName);
}

void UCharacterDetailsUserWidget::OnCharacterIconChanged(UTexture2D* NewCharacterIcon)
{
	if (!IsValid(NewCharacterIcon) || !IsValid(StatsRadar)) return;
	StatsRadar->SetCharacterIcon(NewCharacterIcon);
}

void UCharacterDetailsUserWidget::OnLevelChanged(int32 NewLevel)
{
	if (!IsValid(TextBlock_Level)) return;
	TextBlock_Level->SetText(FText::AsNumber(NewLevel));
}

void UCharacterDetailsUserWidget::OnTribeDataChanged(const FTribeData& NewTribeData)
{
	SetTribeName(NewTribeData.TribeName);
}

void UCharacterDetailsUserWidget::SetTribeName(const FText& NewTribeName)
{
	if (!IsValid(TextBlock_TribeName)) return;
	static const FText TribeFormat =
		NSLOCTEXT("StatsPanel", "TribeFormat", "of {0}");

	const FText DisplayText = FText::Format(TribeFormat, NewTribeName);
	TextBlock_TribeName->SetText(DisplayText);
}

void UCharacterDetailsUserWidget::OnStatButtonPressed(const UStatButton* PressedButton) const
{
	if (!IsValid(InventoryWidgetController)) return;

	FGameplayTag StatTag = GetStatTagFromButton(PressedButton);
	if (StatTag.IsValid())
	{
		InventoryWidgetController->UpgradeAttribute(StatTag);
	}
}

FGameplayTag UCharacterDetailsUserWidget::GetStatTagFromButton(const UStatButton* PressedButton) const
{
	for (const auto& KVP : MappedStatBindings)
	{
		if (KVP.Value.StatButton == PressedButton)
		{
			return KVP.Key;
		}
	}
	return FGameplayTag(); // return invalid if not found
}

void UCharacterDetailsUserWidget::PlayStatIncreasedEffects()
{
	if (OnStatBoostAnim)
		PlayAnimation(OnStatBoostAnim);

	if (StatBoostSound)
		UGameplayStatics::PlaySound2D(this, StatBoostSound);
}

void UCharacterDetailsUserWidget::ToggleDropdown()
{
	bExpanded = !bExpanded;

	if (ExpandPassiveBarAnim)
	{
		if (bExpanded)
		{
			PlayAnimationForward(ExpandPassiveBarAnim);
		}
		else
		{
			PlayAnimationReverse(ExpandPassiveBarAnim);
		}
	}
}