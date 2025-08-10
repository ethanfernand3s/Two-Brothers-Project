// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Inventory/SlotUserWidget.h"

#include "AbilitySystem/Data/AbilityType.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "UI/Widget/Inventory/Abilities/AbilityCardUserWidget.h"

void USlotUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AbilityPreference == EAbilityType::NONE)
	{
		Button_Slot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void USlotUserWidget::EmptySlot()
{
	SizeBox_AbilityCardHolder->RemoveChildAt(0);
	if (AbilityPreference != EAbilityType::NONE)
	{
		Button_Slot->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void USlotUserWidget::FillSlot(UAbilityCardUserWidget* NewCard)
{
	if (!SizeBox_AbilityCardHolder->HasAnyChildren())
	{
		Button_Slot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SizeBox_AbilityCardHolder->AddChild(NewCard);
	}
	else
	{
		// TODO: Swap if was from inventory, don't pick if was from floor (this is probably not handled here)
	}
}
