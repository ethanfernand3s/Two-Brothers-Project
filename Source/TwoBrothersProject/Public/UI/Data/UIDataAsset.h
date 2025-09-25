// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "UIDataAsset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCreatureTypeImages
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImageMaps")
	TObjectPtr<UTexture2D> TypeImage_64px;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImageMaps")
	TObjectPtr<UTexture2D> TypeImage_32px;
};

UCLASS()
class TWOBROTHERSPROJECT_API UUIDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	const FLinearColor* GetRarityColor(const FGameplayTag& Key) const;
	UTexture2D* GetGenderTexture(const FGameplayTag& Key) const;
	const FCreatureTypeImages* GetCreatureTypeImages(const FGameplayTag& Key) const;

private:

	UPROPERTY(EditAnywhere, meta=(GameplayTagFilter="Rarities", AllowPrivateAccess))
	TMap<FGameplayTag, FLinearColor> RarityColorMap;

	UPROPERTY(EditAnywhere, meta=(GameplayTagFilter="Genders", AllowPrivateAccess))
	TMap<FGameplayTag, UTexture2D*> GenderTexturesMap;

	UPROPERTY(EditAnywhere, meta=(GameplayTagFilter="CreatureTypes", AllowPrivateAccess))
	TMap<FGameplayTag, FCreatureTypeImages> CreatureTypeImagesMap;
};