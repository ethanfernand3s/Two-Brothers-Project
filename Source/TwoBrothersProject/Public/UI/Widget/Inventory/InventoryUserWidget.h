// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "InventoryUserWidget.generated.h"

class UInventoryWidgetController;
struct FTBAbilityInfo;
class UStatsPanelUserWidget;
class UAttributeMenuUserWidget;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UInventoryUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menus",meta = (BindWidget))
	TObjectPtr<UStatsPanelUserWidget>  StatsPanel;
	
	virtual void SetWidgetController(UObject* InWidgetController) override;

protected:

	virtual void OnWidgetControllerSet() override;

private:

#pragma region Ability Functions
	
	void OnAbilityInfoRecieved(const FTBAbilityInfo& AbilityInfo);
	
#pragma endregion Ability Functions

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
};
