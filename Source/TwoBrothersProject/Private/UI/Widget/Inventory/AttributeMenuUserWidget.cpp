// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Inventory/AttributeMenuUserWidget.h"

#include "TBGameplayTags.h"
#include "AbilitySystem/Data/CreatureType.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/Widget/Inventory/AttributeRowUserWidget.h"
#include "UI/Widget/Inventory/AttributeXPBar.h"
#include "UI/WidgetController/InventoryWidgetController.h"

void UAttributeMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetupAttributeRowTags();
}

void UAttributeMenuUserWidget::SetWidgetController(UObject* InWidgetController)
{
	AttributeRow_Health->SetWidgetController(InWidgetController);
	AttributeRow_Defense->SetWidgetController(InWidgetController);
	AttributeRow_Hunger->SetWidgetController(InWidgetController);
	AttributeRow_Thirst->SetWidgetController(InWidgetController);
	AttributeRow_Oxygen->SetWidgetController(InWidgetController);
	AttributeRow_Speed->SetWidgetController(InWidgetController);
	AttributeRow_Stamina->SetWidgetController(InWidgetController);
	AttributeRow_Strength->SetWidgetController(InWidgetController);
	AttributeRow_Drowsiness->SetWidgetController(InWidgetController);

	AttributeXPBar->SetWidgetController(InWidgetController);
	
	Super::SetWidgetController(InWidgetController);
	
}

void UAttributeMenuUserWidget::OnWidgetControllerSet()
{
	if(auto* InventoryWidgetController = Cast<UInventoryWidgetController>(WidgetController))
	{
		InventoryWidgetController->CurrentAndMax_AttributeInfoDelegate.AddUObject(this,&UAttributeMenuUserWidget::CurrentAndMax_SetAttributeRow);
		InventoryWidgetController->Single_AttributeInfoDelegate.AddUObject(this,&UAttributeMenuUserWidget::Single_SetAttributeRow);
		InventoryWidgetController->Type_AttributeInfoDelegate.AddUObject(this,&UAttributeMenuUserWidget::SetType);

		InventoryWidgetController->OnAttributePointsChangedDelegate.AddUObject(this,&UAttributeMenuUserWidget::OnAttributePointsChanged);
		InventoryWidgetController->OnBiomeChangedDelegate.AddUObject(this,&UAttributeMenuUserWidget::OnBiomeChanged);
		InventoryWidgetController->OnCharacterNameChangedDelegate.AddUObject(this,&UAttributeMenuUserWidget::OnCharacterNameChanged);
		InventoryWidgetController->OnLevelChangedDelegate.AddUObject(this,&UAttributeMenuUserWidget::OnLevelChanged);
		InventoryWidgetController->OnTribeDataChangedDelegate.AddUObject(this,&UAttributeMenuUserWidget::OnTribeDataChanged);
		InventoryWidgetController->OnXpPercentChangedDelegate.AddUObject(this,&UAttributeMenuUserWidget::OnXPPercentChanged);
	}
}

void UAttributeMenuUserWidget::Single_SetAttributeRow(const FTBAttributeInfo& AttributeInfo)
{
	if (const auto* PtrOfAttributeRow = MappedAttributeRows.Find(FGameplayTagContainer(AttributeInfo.AttributeTag)))
	{
		if (auto* AttributeRowCur = *PtrOfAttributeRow)
		{
			AttributeRowCur->TextBlock_AttributeRatio->SetText(
				//FText::Format(FText::FromString(TEXT("{0} %")), FText::AsNumber(AttributeInfo.AttributeValue)));
				FText::AsNumber(AttributeInfo.AttributeValue));
		}
	}
}


void UAttributeMenuUserWidget::CurrentAndMax_SetAttributeRow(const FTBAttributeInfo& CurrentAttributeInfo,
                                                             const FTBAttributeInfo& MaxAttributeInfo)
{
		if (const auto* PtrOfAttributeRow = MappedAttributeRows.Find(FGameplayTagContainer::CreateFromArray(TArray{CurrentAttributeInfo.AttributeTag, MaxAttributeInfo.AttributeTag})))
		{
			if (auto* AttributeRowCur = *PtrOfAttributeRow)
			{
				const float CurrentAttribute{CurrentAttributeInfo.AttributeValue};
				const float MaxAttribute{MaxAttributeInfo.AttributeValue};

				if (MaxAttribute != 0)
				{
					FNumberFormattingOptions Format;
					Format.MinimumFractionalDigits = 1;
					Format.MaximumFractionalDigits = 1;

					FText FormattedAttributeValue = FText::Format(
						NSLOCTEXT("AttributeUI", "AttributeFormat", "{0} / {1}"),
						FText::AsNumber(CurrentAttribute, &Format),
						FText::AsNumber(MaxAttribute, &Format)
					);

					const float AttributePercent = CurrentAttribute / MaxAttribute;
					AttributeRowCur->TextBlock_AttributeRatio->SetText(FormattedAttributeValue);
					AttributeRowCur->ProgressBar_Attribute->SetPercent(AttributePercent);
				}
			}
		}
}

void UAttributeMenuUserWidget::SetType(const FTBAttributeInfo& TypeAttributeInfo)
{
	ECreatureType Type = static_cast<ECreatureType>(static_cast<uint8>(TypeAttributeInfo.AttributeValue));

	if (const UEnum* EnumPtr = StaticEnum<ECreatureType>())
	{
		FText DisplayName = EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(Type));
		TextBlock_Type->SetText(DisplayName);
	}
}


void UAttributeMenuUserWidget::SetupAttributeRowTags()
{
	AttributeRow_Health->AttributeGameplayTag.Add(FTBGameplayTags::Get().Attributes_Health);
	AttributeRow_Health->AttributeGameplayTag.Add(FTBGameplayTags::Get().Attributes_MaxHealth);
	MappedAttributeRows.Add(FGameplayTagContainer::CreateFromArray(AttributeRow_Health->AttributeGameplayTag), AttributeRow_Health);
	
	AttributeRow_Stamina->AttributeGameplayTag.Add(FTBGameplayTags::Get().Attributes_Stamina);
	AttributeRow_Stamina->AttributeGameplayTag.Add(FTBGameplayTags::Get().Attributes_MaxStamina);
	MappedAttributeRows.Add(FGameplayTagContainer::CreateFromArray(AttributeRow_Stamina->AttributeGameplayTag), AttributeRow_Stamina);

	AttributeRow_Oxygen->AttributeGameplayTag.Add(FTBGameplayTags::Get().Attributes_Oxygen);
	AttributeRow_Oxygen->AttributeGameplayTag.Add(FTBGameplayTags::Get().Attributes_MaxOxygen);
	MappedAttributeRows.Add(FGameplayTagContainer::CreateFromArray(AttributeRow_Oxygen->AttributeGameplayTag), AttributeRow_Oxygen);

	AttributeRow_Hunger->AttributeGameplayTag.Add(FTBGameplayTags::Get().Attributes_Hunger);
	AttributeRow_Hunger->AttributeGameplayTag.Add(FTBGameplayTags::Get().Attributes_MaxHunger);
	MappedAttributeRows.Add(FGameplayTagContainer::CreateFromArray(AttributeRow_Hunger->AttributeGameplayTag), AttributeRow_Hunger);

	AttributeRow_Thirst->AttributeGameplayTag.Add(FTBGameplayTags::Get().Attributes_Thirst);
	AttributeRow_Thirst->AttributeGameplayTag.Add(FTBGameplayTags::Get().Attributes_MaxThirst);
	MappedAttributeRows.Add(FGameplayTagContainer::CreateFromArray(AttributeRow_Thirst->AttributeGameplayTag), AttributeRow_Thirst);

	AttributeRow_Drowsiness->AttributeGameplayTag.Add(FTBGameplayTags::Get().Attributes_Drowsiness);
	AttributeRow_Drowsiness->AttributeGameplayTag.Add(FTBGameplayTags::Get().Attributes_MaxDrowsiness);
	MappedAttributeRows.Add(FGameplayTagContainer::CreateFromArray(AttributeRow_Drowsiness->AttributeGameplayTag), AttributeRow_Drowsiness);

	AttributeRow_Strength->AttributeGameplayTag.Add(FTBGameplayTags::Get().Attributes_Strength);
	MappedAttributeRows.Add(FGameplayTagContainer(AttributeRow_Strength->AttributeGameplayTag[0]), AttributeRow_Strength);
	
	AttributeRow_Defense->AttributeGameplayTag.Add(FTBGameplayTags::Get().Attributes_Defense);
	MappedAttributeRows.Add(FGameplayTagContainer(AttributeRow_Defense->AttributeGameplayTag[0]), AttributeRow_Defense);

	AttributeRow_Speed->AttributeGameplayTag.Add(FTBGameplayTags::Get().Attributes_Speed);
	MappedAttributeRows.Add(FGameplayTagContainer(AttributeRow_Speed->AttributeGameplayTag[0]), AttributeRow_Speed);
}

void UAttributeMenuUserWidget::OnAttributePointsChanged(int NewAttributePoints)
{
	AttributeXPBar->TextBlock_PointsAvailable->SetText(FText::AsNumber(NewAttributePoints));
}

void UAttributeMenuUserWidget::OnBiomeChanged(const UBiomeDataAsset* NewBiomeData)
{
	TextBlock_Biome->SetText(NewBiomeData->BiomeName);
}

void UAttributeMenuUserWidget::OnCharacterNameChanged(const FText& NewCharacterName)
{
	TextBlock_PlayerName->SetText(NewCharacterName);
}

void UAttributeMenuUserWidget::OnLevelChanged(int NewLevel)
{
	TextBlock_Level->SetText(FText::AsNumber(NewLevel));
}

void UAttributeMenuUserWidget::OnTribeDataChanged(const FTribeData& NewTribeData)
{
	TextBlock_Tribe->SetText(NewTribeData.TribeName);
}

void UAttributeMenuUserWidget::OnXPPercentChanged(float NewXPPercent)
{
	AttributeXPBar->ProgressBar_XP->SetPercent(NewXPPercent);
}
