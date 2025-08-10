#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "PassiveAbilityDropdownUserWidget.generated.h"

class USlotUserWidget;
class UAbilityCardUserWidget;
class UUniformGridPanel;
class UButton;
class UHorizontalBox;
class UWidgetAnimation;

/**
 * Dropdown panel that shows passive ability slots and expands/collapses on toggle.
 */
UCLASS()
class TWOBROTHERSPROJECT_API UPassiveAbilityDropdownUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	
	/** Button that toggles the dropdown open/closed */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DropdownHandleButton;
	
	void ReceivePassiveAbility(UAbilityCardUserWidget* AbilityCard, int32 PreferredSlotIndex = -1);

protected:
	
	/** passive ability slots */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlotUserWidget> PassiveAbilitySlot_1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlotUserWidget> PassiveAbilitySlot_2;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlotUserWidget> PassiveAbilitySlot_3;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlotUserWidget> PassiveAbilitySlot_4;
};
