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
}

void UAttributeMenuUserWidget::SetWidgetController(UObject* InWidgetController)
{
	AttributeXPBar->SetWidgetController(InWidgetController);
	
	Super::SetWidgetController(InWidgetController);
	
}

void UAttributeMenuUserWidget::OnWidgetControllerSet()
{
	SetupAttributeRowTags();
	
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
	if (UAttributeRowUserWidget ** PtrOfAttributeRow = MappedAttributeRows.Find(FGameplayTagContainer(AttributeInfo.AttributeTag)))
	{
		if (UAttributeRowUserWidget* AttributeRowCur = *PtrOfAttributeRow)
		{
			AttributeRowCur->TextBlock_AttributeRatio->SetText(
				//FText::Format(FText::FromString(TEXT("{0} %")), FText::AsNumber(AttributeInfo.AttributeValue))); // For # % formatting
				FText::AsNumber(AttributeInfo.AttributeValue));
		}
	}
}


void UAttributeMenuUserWidget::CurrentAndMax_SetAttributeRow(const FTBAttributeInfo& CurrentAttributeInfo,
                                                             const FTBAttributeInfo& MaxAttributeInfo)
{
		if (UAttributeRowUserWidget ** PtrOfAttributeRow = MappedAttributeRows.Find(FGameplayTagContainer::CreateFromArray(TArray{CurrentAttributeInfo.AttributeTag, MaxAttributeInfo.AttributeTag})))
		{
			if (UAttributeRowUserWidget* AttributeRowCur = *PtrOfAttributeRow)
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
	// Set Widget(s) with appropriate attribute gameplay tags
}

void UAttributeMenuUserWidget::OnAttributePointsChanged(int NewAttributePoints)
{
	AttributeXPBar->TextBlock_PointsAvailable->SetText(FText::AsNumber(NewAttributePoints));
}

void UAttributeMenuUserWidget::OnBiomeChanged(const FBiomeInfo& NewBiomeInfo)
{
	TextBlock_Biome->SetText(NewBiomeInfo.BiomeName);
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
