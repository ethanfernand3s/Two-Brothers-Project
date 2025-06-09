// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "InventoryUserWidget.generated.h"

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
	TObjectPtr<UAttributeMenuUserWidget>  AttributeMenu;

protected:
	virtual void NativeConstruct() override;
};
