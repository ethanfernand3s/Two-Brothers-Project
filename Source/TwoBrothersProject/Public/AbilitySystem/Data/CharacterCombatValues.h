// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterCombatValues.generated.h"

USTRUCT(BlueprintType)
struct FCharacterCombatValues
{
	GENERATED_BODY()
	
	static constexpr int32 MaxIV = 21;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Strength = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AuraStrength = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Defense = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AuraDefense = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Speed = 0;
	

	/** Normalize value (0.0–1.0) for any stat */
	static float GetNormalized(int32 Value)
	{
		return FMath::Clamp(static_cast<float>(Value) / MaxIV, 0.f, 1.f);
	}

	void SetRandomValues()
	{
		Health = FMath::RandRange(0, 21);
		Strength = FMath::RandRange(0, 21);
		AuraStrength = FMath::RandRange(0, 21);
		Defense = FMath::RandRange(0, 21);
		AuraDefense = FMath::RandRange(0, 21);
		Speed = FMath::RandRange(0, 21);
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