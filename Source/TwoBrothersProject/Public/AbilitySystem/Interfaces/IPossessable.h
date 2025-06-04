// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPossessable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPossessable : public UInterface
{ GENERATED_BODY() };

class IPossessable
{
	GENERATED_BODY()
public:
	/*
	 * Returns a percentage chance of possession 0 being 0% and 1 being 100%
	 */ 
	UFUNCTION()
	virtual bool CanBePossessedBy() const = 0;
};
