// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TBGamemode.generated.h"

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API ATBGamemode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};
