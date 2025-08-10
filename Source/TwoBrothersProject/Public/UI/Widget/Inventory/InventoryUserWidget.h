// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "InventoryUserWidget.generated.h"

class UAbilityCardUserWidget;
class UItemsPanelUserWidget;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menus",meta = (BindWidget))
	TObjectPtr<UItemsPanelUserWidget>  ItemsPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	TSubclassOf<UAbilityCardUserWidget>  Circle_AbilityCardUserWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	TSubclassOf<UAbilityCardUserWidget>  Diamond_AbilityCardUserWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	TSubclassOf<UAbilityCardUserWidget>  Square_AbilityCardUserWidgetClass;
	
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
