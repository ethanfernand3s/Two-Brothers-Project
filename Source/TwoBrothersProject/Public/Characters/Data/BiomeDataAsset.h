// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BiomeDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TWOBROTHERSPROJECT_API UBiomeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Biome")
	FText BiomeName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Biome")
	float Temperature = 70.f;
};
