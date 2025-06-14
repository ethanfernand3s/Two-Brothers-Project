// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECharacterGender : uint8
{
	None		UMETA(DisplayName = "None"),
	Male		UMETA(DisplayName = "Male"), 
	Female		UMETA(DisplayName = "Female"),
};
