// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Components/TBInventoryComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryStatics.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UInventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	static UTBInventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);
	static FGameplayTag GetItemCategoryFromItemComp(const UTBItemComponent* ItemComp);
	static UInventoryUserWidget* GetInventoryWidget(const APlayerController* PC);
};
