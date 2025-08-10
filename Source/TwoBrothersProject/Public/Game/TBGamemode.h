// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TBGamemode.generated.h"

class ACrashLandingShipActor;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API ATBGamemode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	FVector GetRandomIntroSpawnLocation();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	UPROPERTY(EditDefaultsOnly)
	  UClass* CrashLandingShipClass;
	
};
