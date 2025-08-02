// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TBGamemode.h"
#include "Player/ParasitePlayerState.h" // Replace with your actual PlayerState class
#include "Player/TBPlayerController.h"

void ATBGamemode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AParasitePlayerState* PS = Cast<AParasitePlayerState>(NewPlayer->PlayerState);
	if (PS && PS->bIsFirstServerEnter)
	{
		PS->bIsFirstServerEnter = false;
		PS->ForceNetUpdate();  

		ATBPlayerController* PC = Cast<ATBPlayerController>(NewPlayer);
		if (PC)
		{
			//PC->ClientPlayIntroLanding(); // ✅ Safe client RPC call
		}
	}
}
