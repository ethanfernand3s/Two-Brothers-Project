#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "CharacterPanelUserWidget.generated.h"


class UWidgetSwitcher;
class UButton;
class UInventoryWidgetController;
class UCharacterDetailsUserWidget;

UCLASS()
class TWOBROTHERSPROJECT_API UCharacterPanelUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	
	UCharacterDetailsUserWidget* GetParasiteDetailsUserWidget() const {return ParasiteDetailsUserWidget;}
	UCharacterDetailsUserWidget* GetAnimalDetailsUserWidget() const {return ParasiteDetailsUserWidget;}

protected:
	
	virtual void NativeOnInitialized() override;
	virtual void OnWidgetControllerSet() override;
	virtual void OnWidgetControllerRebound(bool bIsAnimalInhabited) override;

	void ResolveAnimalInhabitance(bool bIsAnimalInhabited);
	UFUNCTION()
	void SwitchCharacterPreview();

private:
	
	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(meta=(BindWidget))
	UButton* Button_SwitchCharacter;

	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* WidgetSwitcher_DetailSwitcher;

	UPROPERTY(meta=(BindWidget))
	UCharacterDetailsUserWidget* ParasiteDetailsUserWidget;
	UPROPERTY(meta=(BindWidget))
	UCharacterDetailsUserWidget* AnimalDetailsUserWidget;

	template <typename WidgetType, typename Func, typename... Args>
	void ForwardToChild(bool bIsParasiteVal, Func WidgetFunc, Args&&... args)
	{
		WidgetType* TargetWidget = bIsParasiteVal ? ParasiteDetailsUserWidget : AnimalDetailsUserWidget;
		if (TargetWidget)
		{
			(TargetWidget->*WidgetFunc)(Forward<Args>(args)...);
		}
	}
};

