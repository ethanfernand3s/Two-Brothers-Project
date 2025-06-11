// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreatureType.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"


USTRUCT()
struct FTBAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag AttributeTag;
	
	UPROPERTY()
	FText AttributeName;

	UPROPERTY()
	FText AttributeDescription;

	UPROPERTY()
	float AttributeValue = 0.0f;

	FTBAttributeInfo() = default;
	FTBAttributeInfo(const FGameplayTag& InAttributeTag, const FText& InName, const FText& InDesc, float InValue)
		: AttributeTag(InAttributeTag), AttributeName(InName), AttributeDescription(InDesc), AttributeValue(InValue){}
};

class TWOBROTHERSPROJECT_API AttributeInfoLibrary
{
public:
	static FTBAttributeInfo FindAttributeInfo(const FGameplayTag& Tag);

private:
	static const TMap<FGameplayTag, FTBAttributeInfo>& GetAttributeMap();
};


