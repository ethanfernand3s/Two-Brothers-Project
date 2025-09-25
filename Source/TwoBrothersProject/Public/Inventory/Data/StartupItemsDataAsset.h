// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Inventory/Items/TBInventoryItem.h"
#include "StartupItemsDataAsset.generated.h"

class AItemPickupActor;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UStartupItemsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, meta = (Categories = Items), meta=(RequiredAssetDataTags="Items"))
	TArray<TSubclassOf<AItemPickupActor>> StartupItemClasses;
};
