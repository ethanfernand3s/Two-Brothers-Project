// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ParasitePlayerState.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"
#include "Characters/Components/CharacterContextComponent.h"
#include "Inventory/Components/TBInventoryComponent.h"
#include "Net/UnrealNetwork.h"

AParasitePlayerState::AParasitePlayerState()
{ 
	ParasiteAbilitySystem =	CreateDefaultSubobject<UParasiteAbilitySystemComponent>(TEXT("ParasiteAbilitySystemComponent"));
	ParasiteAttributeSet = CreateDefaultSubobject<UParasiteAttributeSet>(TEXT("ParasiteAttributeSet"));

	ParasiteAbilitySystem->SetIsReplicated(true);
	ParasiteAbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	ParasiteAbilitySystem->AddAttributeSetSubobject(ParasiteAttributeSet);

	CharacterContextComponent = CreateDefaultSubobject<UCharacterContextComponent>(TEXT("CharacterContextComponent"));
	
	bIsInitialised = false;
	SetNetUpdateFrequency(100.f);
}

void AParasitePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AParasitePlayerState,bIsInitialised);
}

void AParasitePlayerState::LoadProgress()
{
	// TODO: Add Loading From Save!!
	
	if (!bIsInitialised)
	{
		ParasiteAbilitySystem->SetBaseStats(FCharacterCombatValues(50, 50, 50, 50, 50, 50));
		CharacterContextComponent->InitializeCharacterContext(ParasiteAttributeSet->CalculateCombatPower());
		ParasiteAbilitySystem->AddIvsToAttributes(CharacterContextComponent->GetIVSet());
		
		bIsInitialised = true;
	}
	 
}

UAbilitySystemComponent* AParasitePlayerState::GetAbilitySystemComponent() const
{
	return ParasiteAbilitySystem;
}

UParasiteAttributeSet* AParasitePlayerState::GetParasiteAttributeSet() const
{
	return ParasiteAttributeSet;
}

const TArray<UGameplayEffect*> AParasitePlayerState::GetBuffForHost()
{
	// TODO: Add Buffs Later
	return TArray<UGameplayEffect*>();
}

UCharacterContextComponent* AParasitePlayerState::GetCharacterContextComponent() const
{
	return CharacterContextComponent;
}

bool AParasitePlayerState::GetIsInhabited() const
{
	return false; // Will never be inhabited
}

float AParasitePlayerState::GetXPMultiplierAmount()
{
	// TODO: Adjust based on current Multiplier Bonuses
	return 1;
}