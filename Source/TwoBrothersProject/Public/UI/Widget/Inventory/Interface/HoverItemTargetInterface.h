// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HoverItemTargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHoverItemTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TWOBROTHERSPROJECT_API IHoverItemTargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HandleOpposingClick(class UHoverItemUserWidget* HoverWidget) = 0;
};
