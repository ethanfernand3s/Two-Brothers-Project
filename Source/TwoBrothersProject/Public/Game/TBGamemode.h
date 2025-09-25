// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TBGamemode.generated.h"

class AParasiteCharacter;
class UUIDataAsset;
class ACrashLandingShipActor;
class URarityDataAsset;
class UGenderDataAsset;
class UCreatureTypeDataAsset;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API ATBGamemode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void SpawnCrashLandingShipWithParasite(APlayerController* NewPlayer) const;
	static FVector GetRandomIntroSpawnLocation(const FVector& Min, const FVector& Max);

private:
	
	/* ================= Classes ================= */
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="CrashShip")
	TSubclassOf<ACrashLandingShipActor> CrashLandingShipClass;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="CrashShip")
	FVector ShipSpawnLocation_TESTING_Max = FVector(-9750.0f, 300.0f, 6000.0f);
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="CrashShip")
	FVector ShipSpawnLocation_TESTING_Min = FVector(-11000.0f, 0.0f, 5000.0f);
	
};
