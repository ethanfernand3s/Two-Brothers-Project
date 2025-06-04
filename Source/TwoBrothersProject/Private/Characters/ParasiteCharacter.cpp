// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ParasiteCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Player/ParasitePlayerState.h"
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

void AParasiteCharacter::InitStatusBar()
{
	if (StatusBarWidgetComponent)
	{
		if (UStatusBarUserWidget* StatusBarUserWidget = Cast<UStatusBarUserWidget>(StatusBarWidgetComponent->GetUserWidgetObject()))
		{
			if (AParasitePlayerState* PS = GetPlayerState<AParasitePlayerState>())
			{
				StatusBarUserWidget->SetAttributeSet(PS->GetParasiteAttributeSet());
			}
		}
	}
}

void AParasiteCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	
	if (AParasitePlayerState* PS = GetPlayerState<AParasitePlayerState>())
	{
		if (UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent())
		{
			ASC->InitAbilityActorInfo(PS,this);
			if (HasAuthority())
			{
				PS->EnsureInitialAttributeDefaults();
				PS->EnsureAbilitiesAreInitialized();
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
