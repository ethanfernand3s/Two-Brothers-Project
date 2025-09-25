// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySet.h"
#include "BaseAbilitySet.generated.h"

USTRUCT()
struct FAbilitySet_Ability
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayAbility> AbilityClass;
	UPROPERTY(EditDefaultsOnly) int32 AbilityLevel = 1;
};

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UBaseAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FAbilitySet_Ability> Abilities;
};
