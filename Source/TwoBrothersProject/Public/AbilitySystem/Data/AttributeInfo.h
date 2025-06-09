// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreatureType.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"


USTRUCT()
struct FTBFloatAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FText AttributeName;

	UPROPERTY()
	FText AttributeDescription;

	UPROPERTY()
	float AttributeValue = 0.0f;

	FTBFloatAttributeInfo() = default;
	FTBFloatAttributeInfo(const FText& InName, const FText& InDesc, float InValue)
		: AttributeName(InName), AttributeDescription(InDesc), AttributeValue(InValue){}
};

USTRUCT()
struct FTBCreatureTypeAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FText AttributeName = FText::FromString(TEXT("Creature Type"));

	UPROPERTY()
	FText AttributeDescription = FText::FromString(TEXT("Current creature type"));

	// Default type for the moment adjust if needed later
	UPROPERTY()
	ECreatureType AttributeValue = ECreatureType::Parasite;

	FTBCreatureTypeAttributeInfo() = default;
};

class TWOBROTHERSPROJECT_API AttributeInfoLibrary
{
public:
	static FTBFloatAttributeInfo FindFloatAttributeInfo(const FGameplayTag& Tag);
	static FTBCreatureTypeAttributeInfo GetCreatureTypeAttribute();

private:
	static const TMap<FGameplayTag, FTBFloatAttributeInfo>& GetFloatAttributeMap();
};


