#include "UI/Widget/Inventory/Stats/StatsPanelUserWidget.h"
#include "UI/Widget/Inventory/Stats/StatsRadarWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TBGameplayTags.h"
#include "AbilitySystem/Data/CreatureType.h"
#include "AbilitySystem/Data/CreatureTypeDataAsset.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/Data/GenderDataAsset.h"
#include "Components/TextBlock.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "UI/Widget/Inventory/SlotUserWidget.h"
#include "UI/Widget/Inventory/Abilities/AbilityCardUserWidget.h"
#include "UI/Widget/Inventory/Abilities/PassiveAbilityDropdownUserWidget.h"
#include "UI/Widget/Inventory/Stats/StatButton.h"
#include "UI/Widget/Inventory/Stats/StatsRadarUserWidget.h"
#include "UI/WidgetController/InventoryWidgetController.h"

void UStatsPanelUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStatsPanelUserWidget::SetWidgetController(UObject* InWidgetController)
{
	Super::SetWidgetController(InWidgetController);
}

void UStatsPanelUserWidget::ReceiveDefaultAbility(UAbilityCardUserWidget* AbilityCard, int32 PreferredSlotIndex)
{
	// Autofill slots
	if (PreferredSlotIndex == -1)
	{
		if (!DefaultAbilitySlot_1->SizeBox_AbilityCardHolder->HasAnyChildren())
		{
			DefaultAbilitySlot_1->FillSlot(AbilityCard);
		}
		else if (!DefaultAbilitySlot_2->SizeBox_AbilityCardHolder->HasAnyChildren())
		{
			DefaultAbilitySlot_2->FillSlot(AbilityCard);
		}
		else if (!DefaultAbilitySlot_3->SizeBox_AbilityCardHolder->HasAnyChildren())
		{
			DefaultAbilitySlot_3->FillSlot(AbilityCard);
		}
		else if (!DefaultAbilitySlot_4->SizeBox_AbilityCardHolder->HasAnyChildren())
		{
			DefaultAbilitySlot_4->FillSlot(AbilityCard);
		}
		
	}
	// Manually fill
	else
	{
		switch (PreferredSlotIndex)
		{
			case 0:
				DefaultAbilitySlot_1->FillSlot(AbilityCard);
				break;
			case 1:
				DefaultAbilitySlot_2->FillSlot(AbilityCard);
				break;
			case 2:
				DefaultAbilitySlot_3->FillSlot(AbilityCard);
				break;
			case 3:
				DefaultAbilitySlot_4->FillSlot(AbilityCard);
				break;
			default: break;
		}
	}
}



void UStatsPanelUserWidget::ReceiveMainAbility(UAbilityCardUserWidget* AbilityCard)
{
	MainAbilitySlot->FillSlot(AbilityCard);
}

void UStatsPanelUserWidget::OnWidgetControllerSet()
{
	if(InventoryWidgetController = Cast<UInventoryWidgetController>(WidgetController))
	{
		// Attribute Delegates
		InventoryWidgetController->CurrentAndMax_AttributeInfoDelegate.AddUObject(this, &UStatsPanelUserWidget::SetHealthStat);
		InventoryWidgetController->Single_AttributeInfoDelegate.AddUObject(this, &UStatsPanelUserWidget::SetSingleStat);
		
		//Character Context Delegates
		InventoryWidgetController->OnAttributePointsChangedDelegate.AddUObject(this,&UStatsPanelUserWidget::OnAttributePointsChanged);
		InventoryWidgetController->OnCharacterNameChangedDelegate.AddUObject(this,&UStatsPanelUserWidget::OnCharacterNameChanged);
		InventoryWidgetController->OnLevelChangedDelegate.AddUObject(this,&UStatsPanelUserWidget::OnLevelChanged);
		InventoryWidgetController->OnTribeNameChangedDelegate.AddUObject(this, &UStatsPanelUserWidget::OnTribeDataChanged);
		InventoryWidgetController->OnGenderSetDelegate.AddUObject(this, &UStatsPanelUserWidget::OnGenderSet);
		InventoryWidgetController->OnAuraColorSetDelegate.AddUObject(this, &UStatsPanelUserWidget::OnAuraColorSet);
		InventoryWidgetController->OnCreatureTypesSetDelegate.AddUObject(this,&UStatsPanelUserWidget::SetType);
	}

	SetupStatButtons();

	if (PassiveAbilityDropdown && PassiveAbilityDropdown->DropdownHandleButton)
	{
		PassiveAbilityDropdown->DropdownHandleButton->OnClicked.AddDynamic(this, &UStatsPanelUserWidget::ToggleDropdown);
	}
}

void UStatsPanelUserWidget::SetupStatButtons()
{
	// On Clicked Delegates
	StatButton_Health->Button_Stat->OnClicked.AddDynamic(this, &UStatsPanelUserWidget::OnStatButtonPressed_Health);
	StatButton_Strength->Button_Stat->OnClicked.AddDynamic(this, &UStatsPanelUserWidget::OnStatButtonPressed_Strength);
	StatButton_Defense->Button_Stat->OnClicked.AddDynamic(this, &UStatsPanelUserWidget::OnStatButtonPressed_Defense);
	StatButton_AuraStrength->Button_Stat->OnClicked.AddDynamic(this, &UStatsPanelUserWidget::OnStatButtonPressed_AuraStrength);
	StatButton_AuraDefense->Button_Stat->OnClicked.AddDynamic(this, &UStatsPanelUserWidget::OnStatButtonPressed_AuraDefense);
	StatButton_Speed->Button_Stat->OnClicked.AddDynamic(this, &UStatsPanelUserWidget::OnStatButtonPressed_Speed);

	// Map stats for quick lookup
	const auto Tags = FTBGameplayTags::Get();
	MappedStatBindings.Add(Tags.Attributes_Health,       {StatButton_Health,       0});
	MappedStatBindings.Add(Tags.Attributes_Strength,     {StatButton_Strength,     1});
	MappedStatBindings.Add(Tags.Attributes_Defense,      {StatButton_Defense,      2});
	MappedStatBindings.Add(Tags.Attributes_Speed,        {StatButton_Speed,        3});
	MappedStatBindings.Add(Tags.Attributes_AuraDefense,  {StatButton_AuraDefense,  4});
	MappedStatBindings.Add(Tags.Attributes_AuraStrength, {StatButton_AuraStrength, 5});	
}

void UStatsPanelUserWidget::SetSingleStat(const FTBAttributeInfo& AttributeInfo)
{
	if (FStatUIBinding* Binding = MappedStatBindings.Find(AttributeInfo.AttributeTag))
	{
		const FText StatValueText = FText::AsNumber(AttributeInfo.AttributeValue);
		if (Binding->StatButton)
		{
			Binding->StatButton->TextBlock_Stat->SetText(StatValueText);
		}

		if (StatsRadar && Binding->RadarIndex != INDEX_NONE)
		{
			const float Normalized = FMath::Clamp(AttributeInfo.AttributeValue / FCharacterCombatValues::MaxValueAmount, 0.f, 1.f);
			StatsRadar->UpdateSingleRadarStat(Binding->RadarIndex, Normalized);
		}
		
		PlayStatIncreasedEffects();
	}
}

void UStatsPanelUserWidget::SetHealthStat(const FTBAttributeInfo& CurrentAttributeInfo,	const FTBAttributeInfo& MaxAttributeInfo)
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
		
		StatButton_Health->TextBlock_Stat->SetText(FormattedAttributeValue);

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

void UStatsPanelUserWidget::SetType(const TPair<ECreatureType, ECreatureType>& CreatureTypes)
{
	if (!InventoryWidgetController || !InventoryWidgetController->CreatureTypeData) return;

	const auto& InfoMap = InventoryWidgetController->CreatureTypeData->CreatureTypeInfoMap;
	
	// Create lambda to process both elements
	auto AddTypeIcon = [&](ECreatureType Type)
	{
		if (Type == ECreatureType::None) return;

		if (const FCreatureTypeInfo* Info = InfoMap.Find(Type))
		{
			if (!Info->TypeImage) return;

			// Prevent duplicates
			for (UWidget* Child : VerticalBox_Types->GetAllChildren())
			{
				if (const UImage* Img = Cast<UImage>(Child))
				{
					if (Img->GetBrush().GetResourceObject() == Info->TypeImage)
						return;
				}
			}

			// Cap at two icons
			if (VerticalBox_Types->GetChildrenCount() >= 2) return;

			UImage* NewImg = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
			NewImg->SetBrushFromTexture(Info->TypeImage, true);
			NewImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			if (UVerticalBoxSlot* Slot = VerticalBox_Types->AddChildToVerticalBox(NewImg))
			{
				Slot->SetHorizontalAlignment(HAlign_Center);
				Slot->SetPadding(FMargin(0.f, 5.f));
			}
		}
	};

	// Process the two elements of the pair
	AddTypeIcon(CreatureTypes.Key);
	AddTypeIcon(CreatureTypes.Value);
}

void UStatsPanelUserWidget::OnGenderSet(const ECharacterGender& InGender)
{
	if (InventoryWidgetController &&  InventoryWidgetController->GenderData)
	{
		if (FGenderInfo* CharacterGenderInfo = InventoryWidgetController->GenderData->GenderInfoMap.Find(InGender))
		{
			Image_Gender->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Image_Gender->SetBrushFromTexture(CharacterGenderInfo->GenderImage);

			// Otherwise they are trying to set gender when they already have one
			// TODO: Decide if we want two types or more or to be able to changed
		}
	}
}

void UStatsPanelUserWidget::OnAuraColorSet(const FColor& InColor)
{
	TextBlock_CharacterName->SetColorAndOpacity(InColor);
}

void UStatsPanelUserWidget::OnAttributePointsChanged(int NewAttributePoints)
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

void UStatsPanelUserWidget::UpdateAllButtons(bool bShouldEnable)
{
	StatButton_Health->Button_Stat->SetIsEnabled(bShouldEnable);
	StatButton_Strength->Button_Stat->SetIsEnabled(bShouldEnable);
	StatButton_Defense->Button_Stat->SetIsEnabled(bShouldEnable);
	StatButton_AuraStrength->Button_Stat->SetIsEnabled(bShouldEnable);
	StatButton_AuraDefense->Button_Stat->SetIsEnabled(bShouldEnable);
	StatButton_Speed->Button_Stat->SetIsEnabled(bShouldEnable);
}

void UStatsPanelUserWidget::OnCharacterNameChanged(const FText& NewCharacterName)
{
	TextBlock_CharacterName->SetText(NewCharacterName);
}

void UStatsPanelUserWidget::OnLevelChanged(int32 NewLevel)
{
	TextBlock_Level->SetText(FText::AsNumber(NewLevel));
}

void UStatsPanelUserWidget::OnTribeDataChanged(const FText& NewTribeText)
{
	static const FText TribeFormat =
		NSLOCTEXT("StatsPanel", "TribeFormat", "of {0}");

	const FText DisplayText = FText::Format(TribeFormat, NewTribeText);
	TextBlock_TribeName->SetText(DisplayText);
}

void UStatsPanelUserWidget::OnStatButtonPressed_Health()
{
	if(InventoryWidgetController)
	{
		InventoryWidgetController->UpgradeAttribute(FTBGameplayTags::Get().Attributes_MaxHealth);
	}
}

void UStatsPanelUserWidget::OnStatButtonPressed_Strength()
{
	if(InventoryWidgetController)
	{
		InventoryWidgetController->UpgradeAttribute(FTBGameplayTags::Get().Attributes_Strength);
	}
}

void UStatsPanelUserWidget::OnStatButtonPressed_Defense()
{
	if(InventoryWidgetController)
	{
		InventoryWidgetController->UpgradeAttribute(FTBGameplayTags::Get().Attributes_Defense);
	}
}

void UStatsPanelUserWidget::OnStatButtonPressed_AuraStrength()
{
	if(InventoryWidgetController)
	{
		InventoryWidgetController->UpgradeAttribute(FTBGameplayTags::Get().Attributes_AuraStrength);
	}
}

void UStatsPanelUserWidget::OnStatButtonPressed_AuraDefense()
{
	if(InventoryWidgetController)
	{
		InventoryWidgetController->UpgradeAttribute(FTBGameplayTags::Get().Attributes_AuraDefense);
	}
}

void UStatsPanelUserWidget::OnStatButtonPressed_Speed()
{
	if(InventoryWidgetController)
	{
		InventoryWidgetController->UpgradeAttribute(FTBGameplayTags::Get().Attributes_Speed);
	}
}

void UStatsPanelUserWidget::PlayStatIncreasedEffects()
{
	if (OnStatBoostAnim)
		PlayAnimation(OnStatBoostAnim);

	if (StatBoostSound)
		UGameplayStatics::PlaySound2D(this, StatBoostSound);
}

void UStatsPanelUserWidget::ToggleDropdown()
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