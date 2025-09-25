#include "UI/Widget/Inventory/CharacterPanel/CharacterPanelUserWidget.h"

#include "Kismet/GameplayStatics.h"
#include "TBGameplayTags.h"
#include "AbilitySystem/Data/CharacterCombatValues.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "UI/Data/UIDataAsset.h"
#include "UI/Widget/Inventory/Slots/SlotUserWidget.h"
#include "UI/Widget/Inventory/CharacterPanel/Stats/StatButton.h"
#include "UI/Widget/Inventory/CharacterPanel/Stats/StatsRadarUserWidget.h"
#include "UI/Widget/Inventory/Slots/SlotContainerUserWidget.h"
#include "UI/Widget/Inventory/Slots/SlotPanelUserWidget.h"
#include "UI/WidgetController/InventoryWidgetController.h"

void UCharacterPanelUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UCharacterPanelUserWidget::OnWidgetControllerSet()
{
	if(InventoryWidgetController = Cast<UInventoryWidgetController>(WidgetController))
	{
		// Attribute Delegates
		InventoryWidgetController->CurrentAndMax_AttributeInfoDelegate.AddUObject(this, &UCharacterPanelUserWidget::SetHealthStat);
		InventoryWidgetController->Single_AttributeInfoDelegate.AddUObject(this, &UCharacterPanelUserWidget::SetSingleStat);
		
		//Character Context Delegates
		InventoryWidgetController->OnAttributePointsChangedDelegate.AddUObject(this,&UCharacterPanelUserWidget::OnAttributePointsChanged);
		InventoryWidgetController->OnCharacterNameChangedDelegate.AddUObject(this,&UCharacterPanelUserWidget::OnCharacterNameChanged);
		InventoryWidgetController->OnLevelChangedDelegate.AddUObject(this,&UCharacterPanelUserWidget::OnLevelChanged);
		InventoryWidgetController->OnTribeNameChangedDelegate.AddUObject(this, &UCharacterPanelUserWidget::OnTribeDataChanged);
		InventoryWidgetController->OnGenderSetDelegate.AddUObject(this, &UCharacterPanelUserWidget::OnGenderSet);
		InventoryWidgetController->OnRaritySetDelegate.AddUObject(this, &UCharacterPanelUserWidget::OnRaritySet);
		InventoryWidgetController->OnCreatureTypesSetDelegate.AddUObject(this,&UCharacterPanelUserWidget::SetType);
	}
	
	if (Button_EquippablePassiveAbilitiesContainerToggle)
	{
		Button_EquippablePassiveAbilitiesContainerToggle->OnClicked.AddDynamic(this, &UCharacterPanelUserWidget::ToggleDropdown);
	}

	SetupStatButtons();
	
	PassivePanel->SetWidgetController(WidgetController);
	MainAbility_SlotContainer->SetWidgetController(WidgetController);
	DefaultAbilities_SlotContainer->SetWidgetController(WidgetController);
}

void UCharacterPanelUserWidget::SetupStatButtons()
{
	// Map stats for quick lookup
	const auto Tags = FTBGameplayTags::Get();
	MappedStatBindings.Add(Tags.Attributes_Health,       {StatButton_Health,       0});
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
			StatButtonPair.Value.StatButton->OnStatButtonClicked.BindUObject(this, &UCharacterPanelUserWidget::OnStatButtonPressed);
		}
	}
}

void UCharacterPanelUserWidget::SetSingleStat(const FTBAttributeInfo& AttributeInfo)
{
	if (FStatUIBinding* Binding = MappedStatBindings.Find(AttributeInfo.AttributeTag))
	{
		const FText StatValueText = FText::AsNumber(AttributeInfo.AttributeValue);
		if (Binding->StatButton)
		{
			Binding->StatButton->SetStatText(StatValueText);
		}

		if (StatsRadar && Binding->RadarIndex != INDEX_NONE)
		{
			const float Normalized = FMath::Clamp(AttributeInfo.AttributeValue / FCharacterCombatValues::MaxValueAmount, 0.f, 1.f);
			StatsRadar->UpdateSingleRadarStat(Binding->RadarIndex, Normalized);
		}
		
		PlayStatIncreasedEffects();
	}
}

void UCharacterPanelUserWidget::SetHealthStat(const FTBAttributeInfo& CurrentAttributeInfo,	const FTBAttributeInfo& MaxAttributeInfo)
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
		
		StatButton_Health->SetStatText(FormattedAttributeValue);

		// Refresh Widget only if Max Health was changed
		if (PreviousMaxHealth != NewMaxHealth)
		{
			PreviousMaxHealth = NewMaxHealth;
			if (StatsRadar)
			{
				const float Normalized = FMath::Clamp(NewMaxHealth / FCharacterCombatValues::MaxValueAmount, 0.f, 1.f);
				StatsRadar->UpdateSingleRadarStat(0, Normalized);
			}
			PlayStatIncreasedEffects();
		}
	}
}

void UCharacterPanelUserWidget::SetType(const FGameplayTagContainer& CreatureTypeTags)
{
	if (!InventoryWidgetController || !InventoryWidgetController->GetUIDataAsset()) return;
	
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

void UCharacterPanelUserWidget::OnGenderSet(const FGameplayTag& InGenderTag)
{
	if (InventoryWidgetController &&  InventoryWidgetController->GetUIDataAsset())
	{
		if (UTexture2D* GenderImage = InventoryWidgetController->GetUIDataAsset()->GetGenderTexture(InGenderTag))
		{
			Image_Gender->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Image_Gender->SetBrushFromTexture(GenderImage);
		}
	}
}

void UCharacterPanelUserWidget::OnRaritySet(const FGameplayTag& InRarityTag)
{
	if (InventoryWidgetController && InventoryWidgetController->GetUIDataAsset())
	{
		if (const FLinearColor* RarityColor =InventoryWidgetController->GetUIDataAsset()->GetRarityColor(InRarityTag))
		{
			TextBlock_CharacterName->SetColorAndOpacity(*RarityColor);
		}
	}
}

void UCharacterPanelUserWidget::OnAttributePointsChanged(int NewAttributePoints)
{
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

void UCharacterPanelUserWidget::UpdateAllButtons(bool bShouldEnable)
{
	for (const auto& StatButtonPair : MappedStatBindings)
	{
		if (IsValid(StatButtonPair.Value.StatButton))
		{
			StatButtonPair.Value.StatButton->SetIsEnabled(bShouldEnable);
		}
	}
}

void UCharacterPanelUserWidget::OnCharacterNameChanged(const FText& NewCharacterName)
{
	TextBlock_CharacterName->SetText(NewCharacterName);
}

void UCharacterPanelUserWidget::OnLevelChanged(int32 NewLevel)
{
	TextBlock_Level->SetText(FText::AsNumber(NewLevel));
}

void UCharacterPanelUserWidget::OnTribeDataChanged(const FText& NewTribeText)
{
	static const FText TribeFormat =
		NSLOCTEXT("StatsPanel", "TribeFormat", "of {0}");

	const FText DisplayText = FText::Format(TribeFormat, NewTribeText);
	TextBlock_TribeName->SetText(DisplayText);
}

void UCharacterPanelUserWidget::OnStatButtonPressed(const FGameplayTag& StatTag) const
{
	if(InventoryWidgetController)
	{
		InventoryWidgetController->UpgradeAttribute(StatTag);
	}
}

void UCharacterPanelUserWidget::PlayStatIncreasedEffects()
{
	if (OnStatBoostAnim)
		PlayAnimation(OnStatBoostAnim);

	if (StatBoostSound)
		UGameplayStatics::PlaySound2D(this, StatBoostSound);
}

void UCharacterPanelUserWidget::ToggleDropdown()
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