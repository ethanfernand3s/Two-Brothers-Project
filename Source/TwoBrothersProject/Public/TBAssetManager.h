// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TBAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UTBAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:

	static UTBAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
