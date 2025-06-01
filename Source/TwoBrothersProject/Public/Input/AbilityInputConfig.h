// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FAbilityInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 *	
 */
UCLASS()
class TWOBROTHERSPROJECT_API UAbilityInputConfig : public UDataAsset
{
	GENERATED_BODY()
 
	public:

	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputAbilityTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAbilityInputAction> AbilityInputActions;
};
