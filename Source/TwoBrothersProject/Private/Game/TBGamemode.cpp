// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TBGamemode.h"
#include "Player/ParasitePlayerState.h" // Replace with your actual PlayerState class
#include "Player/TBPlayerController.h"
#include "Vehicles/StartGame/CrashLandingShipActor.h"

FVector ATBGamemode::GetRandomIntroSpawnLocation()
{
	return FVector::ZeroVector;
}

void ATBGamemode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AParasitePlayerState* PS = Cast<AParasitePlayerState>(NewPlayer->PlayerState);
	if (PS && PS->bIsFirstServerEnter)
	{
		PS->bIsFirstServerEnter = false;
  

		ATBPlayerController* PC = Cast<ATBPlayerController>(NewPlayer);
		if (PC)
		{
			// 1. Spawn crash ship
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = PC;
			SpawnParams.Instigator = PC->GetPawn();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			FVector SpawnLocation = GetRandomIntroSpawnLocation(); // You can implement this
			FRotator SpawnRotation = FRotator::ZeroRotator;

			ACrashLandingShipActor* CrashShip = GetWorld()->SpawnActor<ACrashLandingShipActor>(
				CrashLandingShipClass,
				SpawnLocation,
				SpawnRotation,
				SpawnParams
			);

			if (CrashShip)
			{
				CrashShip->AttachPlayer(PC->GetPawn());

				// Optionally attach camera and set initial motion
				CrashShip->StartCrashSequence();

				PC->ClientPlayIntroLanding_Implementation(); // ✅ Safe client RPC call
			}
		}
	}
}
