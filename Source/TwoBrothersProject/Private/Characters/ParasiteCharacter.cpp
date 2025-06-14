// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ParasiteCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"
#include "Characters/CharacterContextComponent.h"
#include "Characters/Data/Gender.h"
#include "Components/WidgetComponent.h"
#include "Player/ParasitePlayerState.h"
#include "UI/HUD/PlayerHUD.h"
#include "UI/Widget/ProgressBars/StatusBarUserWidget.h"


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

void AParasiteCharacter::LoadProgress()
{
	// Test
	const AParasitePlayerState* ParasitePlayerState = GetPlayerState<AParasitePlayerState>();
	check(ParasitePlayerState);
	ParasitePlayerState->CharacterContextComponent->InitializeCharacterContext(
	FText::FromString("Jaim"),                     // Name
	1,                                             // Level
	0,                                             // XP
	FTribeData(
		FText::FromString("Pinto Basto"),          // Tribe Name
		FText::FromString("The Royal Family Of Portugal"), // Tribe Desc
		nullptr,                                   // Icon (null for now)
		FLinearColor::Red                          // Tribe Color
	),
	ECharacterGender::Male,                                 // Gender
	BiomeDataAsset,                                // UBiomeDataAsset* reference
	0                                              // Attribute Points
	);
}

void AParasiteCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	LoadProgress();
}

