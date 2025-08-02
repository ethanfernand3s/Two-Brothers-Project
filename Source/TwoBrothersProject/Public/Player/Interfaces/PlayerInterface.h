// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

enum class EGrowthRate : uint8;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TWOBROTHERSPROJECT_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual int32 GetXP() const = 0;

	UFUNCTION()
	virtual void AddToXP(int32 InXP) = 0;
	
	UFUNCTION()
	virtual int32 GetBaseXP() const = 0;

	UFUNCTION()
	virtual int32 GetLevel() = 0;
	
	UFUNCTION()
	virtual void AddToPlayerLevel(int32 InPlayerLevel) = 0;

	UFUNCTION()
	virtual int32 GetAttributePointsReward(int32 Level) const = 0;
	
	UFUNCTION()
	virtual void AddToAttributePoints(int32 InAttributePoints) = 0;
	
	UFUNCTION()
	virtual int32 GetAttributePoints() const = 0;

	UFUNCTION()
	virtual EGrowthRate GetGrowthRate() const = 0;

	UFUNCTION()
	virtual bool GetIsInhabited() const = 0;

	UFUNCTION()
	virtual float GetXPMultiplierAmount() = 0;
};
