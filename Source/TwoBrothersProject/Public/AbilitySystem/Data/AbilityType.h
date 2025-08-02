// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	Default	UMETA(DisplayName = "Default"),
	Passive	UMETA(DisplayName = "Passive"),
	Main	UMETA(DisplayName = "Main")
};
