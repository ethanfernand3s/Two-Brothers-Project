// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ParasitePlayerState.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySet.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"
#include "Characters/CharacterContextComponent.h"
#include "Characters/Data/Gender.h"
#include "Net/UnrealNetwork.h"

AParasitePlayerState::AParasitePlayerState()
{ 
	ParasiteAbilitySystem =	CreateDefaultSubobject<UParasiteAbilitySystemComponent>(TEXT("AnimalAbilitySystemComponent"));
	ParasiteAttributeSet = CreateDefaultSubobject<UParasiteAttributeSet>(TEXT("AnimalAttributeSet"));

	ParasiteAbilitySystem->SetIsReplicated(true);
	ParasiteAbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	ParasiteAbilitySystem->AddAttributeSetSubobject(ParasiteAttributeSet);

	CharacterContextComponent = CreateDefaultSubobject<UCharacterContextComponent>(TEXT("CharacterContextComponent"));
	
	bIsInitialised = false;
	SetNetUpdateFrequency(100.f);
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

		//TODO: Set this in character customization screen
		
		CharacterContextComponent->InitializeCharacterContext(
		FText::FromString("The Answer"),                     // Name
		1,                                             // Level
		0,                                             // XP
		FTribeData(
			FText::FromString("Chosen Ones"),          // Tribe Name
			FText::FromString("The Royal Family Of Portugal"), // Tribe Desc
			nullptr,                                   // Icon (null for now)
			FLinearColor::Red                          // Tribe Color
		),
		ECharacterGender::None,                        // Gender
		0
		);
		CharacterContextComponent->SetAuraColor(FColor::Blue);
		
		// End of TODO
		
		ParasiteAbilitySystem->SetBaseStats(FCharacterCombatValues(50, 50, 50, 50, 50, 50));
		CharacterContextComponent->InitializeCombatRelatedVars(ParasiteAttributeSet->CalculateCombatPower());
		ParasiteAbilitySystem->AddIvsToAttributes(CharacterContextComponent->GetIVSet());
	
		EnsureAbilitiesAreInitialized();
		
		bIsInitialised = true;
	}
	 
}

void AParasitePlayerState::EnsureAbilitiesAreInitialized()
{
	if (!HasAuthority()) return;
	
	ParasiteAbilitySystem->AddCharacterAbilities(StartupAbilitySet->Abilities);
	ParasiteAbilitySystem->AddCharacterPassiveAbilities(StartupPassiveAbilitySet->Abilities);
}