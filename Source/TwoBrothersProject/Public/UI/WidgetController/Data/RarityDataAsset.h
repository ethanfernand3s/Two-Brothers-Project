// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UI/Widget/Inventory/Abilities/AbilityCardUserWidget.h"
#include "RarityDataAsset.generated.h"

enum class ERarity : uint8;

UCLASS()
class TWOBROTHERSPROJECT_API URarityDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere)
	TMap<ERarity, FLinearColor> RarityColorMap;
};
