#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "PassiveAbilityDropdownUserWidget.generated.h"

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

protected:
	
	/** Container for passive ability slots */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> UniformGridPanel_PassiveAbilitySlots;
};
