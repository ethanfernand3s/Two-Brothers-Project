// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/BaseUserWidget.h"
#include "SlotPanelUserWidget.generated.h"

class USlotContainerUserWidget;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API USlotPanelUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	
	USlotContainerUserWidget* GetSlotContainer () const { return SlotContainer; }

protected:
	
	virtual void OnWidgetControllerSet() override;
	
private:
	
	UPROPERTY(meta=(BindWidget))
	USlotContainerUserWidget* SlotContainer;
};
