#pragma once

#include "CoreMinimal.h"
#include "../BaseUserWidget.h"
#include "OverlayUserWidget.generated.h"

class UCharacterDetailsOverlayUserWidget;
class UOverlayWidgetController;

UCLASS()
class TWOBROTHERSPROJECT_API UOverlayUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
protected:
	
	virtual void OnWidgetControllerSet() override;

private:

	void HideAnimalDetailsOverlay();
	void SwapOverlayPositions(bool bIsAnimalInhabited);
	
	UPROPERTY() UOverlayWidgetController* OverlayWidgetController = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCharacterDetailsOverlayUserWidget> ParasiteDetailsOverlay;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCharacterDetailsOverlayUserWidget> AnimalDetailsOverlay;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> SwapOverlayPositionsAnim;
	bool bPositionNeedsSwap{false};

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
