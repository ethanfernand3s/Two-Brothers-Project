// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ParasiteCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "Characters/CharacterContextComponent.h"
#include "Player/ParasitePlayerState.h"
#include "UI/HUD/PlayerHUD.h"


class AParasitePlayerState;

AParasiteCharacter::AParasiteCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void AParasiteCharacter::UnPossessed()
{
	Super::UnPossessed();
	//Play anim of attaching to host
}

void AParasiteCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	const TObjectPtr<AParasitePlayerState> ParasitePlayerState = GetPlayerState<AParasitePlayerState>();
	check(ParasitePlayerState);
	
	if (ParasitePlayerState)
	{
		if (ParasiteAbilitySystemComponent = Cast<UParasiteAbilitySystemComponent>(ParasitePlayerState->GetAbilitySystemComponent()))
		{
			ParasiteAbilitySystemComponent->InitAbilityActorInfo(ParasitePlayerState,this);
			ParasiteAbilitySystemComponent->AbilityActorInfoSet();
			if (HasAuthority())
			{
				ParasitePlayerState->EnsureInitialAttributeDefaults();
				ParasitePlayerState->EnsureAbilitiesAreInitialized();
			}
			else
			{
				APlayerController* PlayerController = GetController<APlayerController>();
				if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
				{
					PlayerHUD->InitOverlay(PlayerController);
				}
			}
		}
	}

	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("CLIENT: Parasites Possessed By Has Ran"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SERVER: Parasites Possessed By Has Ran"));
	}
}

