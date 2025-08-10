// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityType.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Rarity/Data/Rarity.h"
#include "AbilityInfo.generated.h"


enum class ECreatureType : uint8;
enum class ERarity : uint8;
class UGameplayAbility;
 
USTRUCT(BlueprintType)
struct FTBAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		  Category="Ability|Tags",
		  meta = (Categories="Abilities"))
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
			  Category="Ability|Tags",
			  meta = (Categories="Inputs"))
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
			  Category="Ability|Tags",
			  meta = (Categories="Status"))
	FGameplayTag StatusTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
			  Category="Ability|Tags",
			  meta = (Categories="Cooldowns"))
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
			  Category="Ability|Tags",
			  meta = (Categories="Sockets"))
	FGameplayTag SocketRequirementTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		  Category      = "Ability|Types",
		  meta          = (Categories="CreatureTypes"))
	FGameplayTagContainer AllowedCreatureTypes = FGameplayTagContainer();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		  Category      = "Ability|Types",
		  meta          = (Categories="CreatureTypes"))
	FGameplayTag AbilityCreatureTypeTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AbilityDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentXP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 XPRequired = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERarity Rarity = ERarity::Common;

	// Default, passive, or main
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAbilityType AbilityType = EAbilityType::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHeld = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOnHotbar = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;
};

UCLASS()
class TWOBROTHERSPROJECT_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = Abilities))
	TMap<FGameplayTag, FTBAbilityInfo> AbilityMap;
};


