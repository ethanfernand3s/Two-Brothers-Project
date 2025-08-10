#include "UI/Widget/Inventory/Abilities/PassiveAbilityDropdownUserWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/SizeBox.h"
#include "UI/Widget/Inventory/SlotUserWidget.h"


void UPassiveAbilityDropdownUserWidget::ReceivePassiveAbility(UAbilityCardUserWidget* AbilityCard, int32 PreferredSlotIndex)
{
	// Autofill slots
	if (PreferredSlotIndex == -1)
	{
		if (!PassiveAbilitySlot_1->SizeBox_AbilityCardHolder->HasAnyChildren())
		{
			PassiveAbilitySlot_1->FillSlot(AbilityCard);
		}
		else if (!PassiveAbilitySlot_2->SizeBox_AbilityCardHolder->HasAnyChildren())
		{
			PassiveAbilitySlot_2->FillSlot(AbilityCard);
		}
		else if (!PassiveAbilitySlot_3->SizeBox_AbilityCardHolder->HasAnyChildren())
		{
			PassiveAbilitySlot_3->FillSlot(AbilityCard);
		}
		else if (!PassiveAbilitySlot_4->SizeBox_AbilityCardHolder->HasAnyChildren())
		{
			PassiveAbilitySlot_4->FillSlot(AbilityCard);
		}
	}
	// Manually fill
	else
	{
		switch (PreferredSlotIndex)
		{
			case 0:
				PassiveAbilitySlot_1->FillSlot(AbilityCard);
				break;
			case 1:
				PassiveAbilitySlot_2->FillSlot(AbilityCard);
				break;
			case 2:
				PassiveAbilitySlot_3->FillSlot(AbilityCard);
				break;
			case 3:
				PassiveAbilitySlot_4->FillSlot(AbilityCard);
				break;
			default: break;
		}
	}
}
