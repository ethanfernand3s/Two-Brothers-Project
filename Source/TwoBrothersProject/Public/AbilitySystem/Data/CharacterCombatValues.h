// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterCombatValues.generated.h"

USTRUCT(BlueprintType)
struct FCharacterCombatValues
{
	GENERATED_BODY()
	
	static constexpr int32 MaxIVs = 21;
	static constexpr int32 MaxValueAmount = 255;

	UPROPERTY()
	int32 Health = 0;
	
	UPROPERTY()
	int32 Strength = 0;

	UPROPERTY()
	int32 AuraStrength = 0;

	UPROPERTY()
	int32 Defense = 0;

	UPROPERTY()
	int32 AuraDefense = 0;

	UPROPERTY()
	int32 Speed = 0;

    TArray<float> GetNormalized() const
    {
        constexpr float Max = static_cast<float>(MaxValueAmount);
        return {
            Health       / Max,
            Strength     / Max,
            AuraStrength / Max,
            Defense      / Max,
            AuraDefense  / Max,
            Speed        / Max
        };
    }

	void SetRandomIVValues()
	{
		Health = FMath::RandRange(0, MaxIVs);
		Strength = FMath::RandRange(0, MaxIVs);
		AuraStrength = FMath::RandRange(0, MaxIVs);
		Defense = FMath::RandRange(0, MaxIVs);
		AuraDefense = FMath::RandRange(0, MaxIVs);
		Speed = FMath::RandRange(0, MaxIVs);
	}

	int32 GetTotal() const
	{
		return Health + Strength + AuraStrength + Defense + AuraDefense + Speed;
	}

	FCharacterCombatValues() = default;

	FCharacterCombatValues(int32 InHealth, int32 InStrength, int32 InAuraStrength,
		int32 InDefense, int32 InAuraDefense, int32 InSpeed) :
			Health(InHealth), Strength(InStrength), AuraStrength(InAuraStrength),Defense(InDefense), AuraDefense(InAuraDefense), Speed(InSpeed){};
};