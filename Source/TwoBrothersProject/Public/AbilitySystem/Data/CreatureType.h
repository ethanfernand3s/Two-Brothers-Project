// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECreatureType : uint8
{
	Parasite	UMETA(DisplayName = "Parasite"),
	Fire		UMETA(DisplayName = "Fire"),
	Water		UMETA(DisplayName = "Water"),
	Earth		UMETA(DisplayName = "Earth"),
	Wind		UMETA(DisplayName = "Wind"),
	Spirit		UMETA(DisplayName = "Spirit")
};
