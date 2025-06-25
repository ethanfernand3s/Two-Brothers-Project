// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGrowthRate : uint8
{
	Fast          UMETA(DisplayName = "Fast"),
	MediumFast    UMETA(DisplayName = "Medium Fast"),
	MediumSlow    UMETA(DisplayName = "Medium Slow"),
	Slow          UMETA(DisplayName = "Slow"),
	Erratic       UMETA(DisplayName = "Erratic"),
	Fluctuating   UMETA(DisplayName = "Fluctuating"),
	MAX           UMETA(Hidden)
};
