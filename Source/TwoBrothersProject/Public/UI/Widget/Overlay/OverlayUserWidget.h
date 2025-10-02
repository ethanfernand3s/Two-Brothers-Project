#pragma once

#include "CoreMinimal.h"
#include "../BaseUserWidget.h"
#include "OverlayUserWidget.generated.h"

class USlotContainerUserWidget;
class UCharacterDetailsOverlayUserWidget;
class UOverlayWidgetController;

UCLASS()
class TWOBROTHERSPROJECT_API UOverlayUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void SetWidgetController(UBaseWidgetController* InWidgetController) override;
	
protected:
	
	virtual void OnWidgetControllerSet() override;
	virtual void OnWidgetControllerRebound(bool bIsAnimalInhabited) override;

private:
	
	void SwapOverlayPositions(bool bIsAnimalInhabited);
	
	UPROPERTY() UOverlayWidgetController* OverlayWidgetController = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCharacterDetailsOverlayUserWidget> ParasiteDetailsOverlay;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCharacterDetailsOverlayUserWidget> AnimalDetailsOverlay;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlotContainerUserWidget> MainAbility_SlotContainer_Parasite;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlotContainerUserWidget> DefaultAbilities_SlotContainer_Parasite;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlotContainerUserWidget> MainAbility_SlotContainer_Animal;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlotContainerUserWidget> DefaultAbilities_SlotContainer_Animal;
	
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> SwapOverlayPositionsAnim;
	bool bWasAnimalInhabited{false};

	template <typename WidgetType, typename Func, typename... Args>
	void ForwardToChild(bool bIsParasiteVal, Func WidgetFunc, Args&&... args)
	{
		WidgetType* TargetWidget = bIsParasiteVal ? ParasiteDetailsOverlay : AnimalDetailsOverlay;
		if (TargetWidget)
		{
			(TargetWidget->*WidgetFunc)(Forward<Args>(args)...);
		}
	}
};
