// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetUtils.generated.h"

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UWidgetUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	static int32 GetIndexFromPosition(const FIntPoint& Position, int32 Columns);
};
