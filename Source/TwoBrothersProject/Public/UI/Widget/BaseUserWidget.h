// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUserWidget.generated.h"

class UBaseWidgetController;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void SetWidgetController(UBaseWidgetController* InWidgetController);
	
	UPROPERTY()
	TObjectPtr<UBaseWidgetController> WidgetController;

protected:

	virtual void OnWidgetControllerSet();
	
};
