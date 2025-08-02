// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AbilityType.h"
#include "Engine/DataAsset.h"
#include "AbilityCardDataAsset.generated.h"

enum class ERarity : uint8;

USTRUCT(BlueprintType)
struct FRarityAppearanceInfo
{
	GENERATED_BODY()
	
	/* ─ Border base ─ */
	UPROPERTY(EditAnywhere, Category="Border")
	UMaterialInterface* Gradient = nullptr; // For inside of border and optionally the text 

	UPROPERTY(EditAnywhere, Category="Header")
	FLinearColor Tint = FLinearColor::White; 

	/* ─ Glow ring (shown on interaction, matches rarity color) ─ */
	UPROPERTY(EditAnywhere, Category="Glow")
	UMaterialInterface* GlowMaterial = nullptr; // gradient / emissive mat	
};
UCLASS()
class TWOBROTHERSPROJECT_API UAbilityCardDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	// Border appearance
	UPROPERTY(EditAnywhere)
	TMap<EAbilityType, UTexture2D*> AbilityBorderTypeMap;

	UPROPERTY(EditAnywhere)
	TMap<ERarity, FRarityAppearanceInfo> RarityAppearanceMap;
};
