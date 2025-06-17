// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BiomeDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FBiomeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Biome")
	FText BiomeName;

	//TODO: Need to change from a Biome DA because changing vals like this
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Biome")
	float Temperature = 70.f;
};

UCLASS(BlueprintType)
class TWOBROTHERSPROJECT_API UBiomeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	FBiomeInfo BiomeInfo;
};
