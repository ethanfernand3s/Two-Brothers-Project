// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TBGamemode.h"

#include "Characters/ParasiteCharacter.h"
#include "Player/ParasitePlayerState.h" // Replace with your actual PlayerState class
#include "Player/TBPlayerController.h"
#include "Vehicles/StartGame/CrashLandingShipActor.h"

void ATBGamemode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AParasitePlayerState* PS = Cast<AParasitePlayerState>(NewPlayer->PlayerState);
	if (PS && PS->bIsFirstServerEnter)
	{
		PS->bIsFirstServerEnter = false;
		SpawnCrashLandingShipWithParasite(NewPlayer);
	}
}

// TODO: Call this in singleplayer as well (IE. no post login)
void ATBGamemode::SpawnCrashLandingShipWithParasite(APlayerController* PC) const
{
	if (IsValid(PC))
	{
		AParasiteCharacter* ParasiteCharacter = Cast<AParasiteCharacter>(PC->GetPawn());
		if (!IsValid(ParasiteCharacter)) return;
		
		// Spawn & possess crash ship
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = PC;
		SpawnParams.Instigator = ParasiteCharacter;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FVector SpawnLocation = GetRandomIntroSpawnLocation(ShipSpawnLocation_TESTING_Min, ShipSpawnLocation_TESTING_Max);
		FRotator SpawnRotation = FRotator::ZeroRotator; // Need to set to tilt down
		
		ACrashLandingShipActor* ShipActor = GetWorld()->SpawnActor<ACrashLandingShipActor>(
			CrashLandingShipClass,
			SpawnLocation,
			SpawnRotation,
			SpawnParams
		);
		if (!IsValid(ShipActor)) return;

		// TODO: Parasite spawns at 0,0,0 then is teleported, need to find a more canonical way of doing this
		ShipActor->AttachPlayer(ParasiteCharacter);
		PC->Possess(ShipActor);
	}
}

FVector ATBGamemode::GetRandomIntroSpawnLocation(const FVector& Min, const FVector& Max)
{
	// TODO: Make better
	return FVector(
			FMath::FRandRange(Min.X, Max.X),
			FMath::FRandRange(Min.Y, Max.Y),
			FMath::FRandRange(Min.Z, Max.Z)
		);
}
