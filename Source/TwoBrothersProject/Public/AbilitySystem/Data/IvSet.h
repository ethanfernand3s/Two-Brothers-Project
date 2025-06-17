// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IvSet.generated.h"

USTRUCT(BlueprintType)
struct FCharacterIVSet
{
	GENERATED_BODY()
	
	static constexpr int32 MaxIV = 21;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Stamina = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Oxygen = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Hunger = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Thirst = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Strength = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Defense = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Drowsiness = 0;

	/** Normalize value (0.0–1.0) for any stat */
	static float GetNormalized(int32 Value)
	{
		return FMath::Clamp(static_cast<float>(Value) / MaxIV, 0.f, 1.f);
	}

	void SetIVs()
	{
		Health = FMath::RandRange(0, 21);
		Stamina = FMath::RandRange(0, 21);
		Oxygen = FMath::RandRange(0, 21);
		Hunger = FMath::RandRange(0, 21);
		Thirst = FMath::RandRange(0, 21);
		Strength = FMath::RandRange(0, 21);
		Defense = FMath::RandRange(0, 21);
		Speed = FMath::RandRange(0, 21);
		Drowsiness = FMath::RandRange(0, 21);
	}
};