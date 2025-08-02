// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GenderDataAsset.generated.h"

enum class ECharacterGender : uint8;

USTRUCT(BlueprintType)
struct FGenderInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImageMaps")
	TObjectPtr<UTexture2D> GenderImage;
};

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UGenderDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImageMaps")
	TMap<ECharacterGender, FGenderInfo> GenderInfoMap;
};
