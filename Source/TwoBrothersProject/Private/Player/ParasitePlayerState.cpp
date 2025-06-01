// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ParasitePlayerState.h"
#include "AbilitySystemGlobals.h"
#include "GameplayAbilitiesModule.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySet.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"
#include "Net/UnrealNetwork.h"

AParasitePlayerState::AParasitePlayerState()
{
	ParasiteAbilitySystem =	CreateDefaultSubobject<UParasiteAbilitySystemComponent>(TEXT("AnimalAbilitySystemComponent"));
	ParasiteAttributeSet = CreateDefaultSubobject<UParasiteAttributeSet>(TEXT("AnimalAttributeSet"));

	ParasiteAbilitySystem->SetIsReplicated(true);
	ParasiteAbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	ParasiteAbilitySystem->AddAttributeSetSubobject(ParasiteAttributeSet);

	bAttributesInitialised = false;
}

UAbilitySystemComponent* AParasitePlayerState::GetAbilitySystemComponent() const
{
	return ParasiteAbilitySystem;
}

UParasiteAttributeSet* AParasitePlayerState::GetParasiteAttributeSet() const
{
	return ParasiteAttributeSet;
}

void AParasitePlayerState::EnsureInitialAttributeDefaults()
{
	if (bAttributesInitialised) return;
	
	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults
	(ParasiteAbilitySystem, "Parasite", /*Level=*/1, /*IsInitialLoad=*/true);
	bAttributesInitialised = true;
}

void AParasitePlayerState::EnsureAbilitiesAreInitialized()
{
	if (GrantedHandles.Num() == 0)
	{
		ParasiteAbilitySet->GiveToAbilitySystem(ParasiteAbilitySystem, &GrantedHandles);
	}
}

const TArray<UGameplayEffect*> AParasitePlayerState::GetBuffForHost()
{
	// Add Buffs Later
	return TArray<UGameplayEffect*>();
}

void AParasitePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AParasitePlayerState,bAttributesInitialised);
}
