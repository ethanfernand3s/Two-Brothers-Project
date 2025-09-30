// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ParasiteCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "Characters/BaseAnimalCharacter.h"
#include "Inventory/Components/TBInventoryComponent.h"
#include "Player/ParasitePlayerState.h"
#include "Player/TBPlayerController.h"
#include "UI/HUD/PlayerHUD.h"


class AParasitePlayerState;

AParasiteCharacter::AParasiteCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AParasiteCharacter::UnPossessed()
{
	Super::UnPossessed();
	//Play anim of attaching to host
}

void AParasiteCharacter::InitActorInfo()
{
	CachedPlayerState = GetPlayerState<AParasitePlayerState>();
	check(IsValid(CachedPlayerState.Get()));

	CachedParasiteAbilitySystemComponent =
		Cast<UParasiteAbilitySystemComponent>(CachedPlayerState.Get()->GetAbilitySystemComponent());
	
	if (!CachedParasiteAbilitySystemComponent.IsValid()) return;
	CachedParasiteAbilitySystemComponent->InitAbilityActorInfo(CachedPlayerState.Get(),this);

	if (HasAuthority())
	{
		//TODO: Create SaveGame Interface and apply it to the player state and the animal character
		CachedPlayerState->LoadProgress();
	}
	
	CachedPlayerController = GetController<ATBPlayerController>();
	if (!CachedPlayerController.IsValid()) return;
	
	if (IsLocallyControlled())
	{
		CachedPlayerHUD = Cast<APlayerHUD>((CachedPlayerController->GetHUD()));
     	if (!CachedPlayerHUD.IsValid()) return;
		
		CachedPlayerHUD->InitUI(CachedPlayerController.Get());
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

bool AParasiteCharacter::GetIsInhabited() const
{
	check(CachedPlayerState.IsValid());
	return CachedPlayerState->GetIsInhabited();
}

float AParasiteCharacter::GetXPMultiplierAmount()
{
	check(CachedPlayerState.IsValid());
	return CachedPlayerState->GetXPMultiplierAmount();
}

UTBInventoryComponent* AParasiteCharacter::GetInventoryComponent() const
{
	return (CachedPlayerController.IsValid()) ? CachedPlayerController->GetInventoryComponent() : nullptr;
}

UCharacterContextComponent* AParasiteCharacter::GetCharacterContextComponent() const
{
	check(CachedPlayerState.IsValid());
	return CachedPlayerState->GetCharacterContextComponent();
}

UAbilitySystemComponent* AParasiteCharacter::GetAbilitySystemComponent() const
{
	return GetPlayerState<AParasitePlayerState>()->GetAbilitySystemComponent();
}
