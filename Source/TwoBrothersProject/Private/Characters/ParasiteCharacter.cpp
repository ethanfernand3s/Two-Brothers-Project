// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ParasiteCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"
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
	
	if (AParasitePlayerState* PS = GetPlayerState<AParasitePlayerState>())
	{
		if (ParasiteAbilitySystemComponent = Cast<UParasiteAbilitySystemComponent>(PS->GetAbilitySystemComponent()))
		{
			ParasiteAbilitySystemComponent->InitAbilityActorInfo(PS,this);
			ParasiteAbilitySystemComponent->AbilityActorInfoSet();
			if (HasAuthority())
			{
				PS->EnsureInitialAttributeDefaults();
				PS->EnsureAbilitiesAreInitialized();
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
