// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * TB Gameplay Tags
 * 
 * Singleton containing native Gameplay Tags
 */
struct FTBGameplayTags
{
	static const FTBGameplayTags& Get() { return GameplayTags;}
	static void InitializeNativeGameplayTags();
	
#pragma region Attributes

	//Creature Type
	FGameplayTag Attributes_Type;

	// Double
	FGameplayTag Attributes_Health;
	FGameplayTag Attributes_MaxHealth;
	
	FGameplayTag Attributes_Thirst;
	FGameplayTag Attributes_MaxThirst;

	FGameplayTag Attributes_Hunger;
	FGameplayTag Attributes_MaxHunger;

	FGameplayTag Attributes_Stamina;
	FGameplayTag Attributes_MaxStamina;

	FGameplayTag Attributes_Oxygen;
	FGameplayTag Attributes_MaxOxygen;

	FGameplayTag Attributes_Drowsiness;
	FGameplayTag Attributes_MaxDrowsiness;

	// Single 
	FGameplayTag Attributes_Defense;
	FGameplayTag Attributes_Strength;
	FGameplayTag Attributes_Speed;
	
#pragma endregion Attributes

#pragma region Abilities

	FGameplayTag Ability_Parasite_SeekHost;
	FGameplayTag Ability_Parasite_TryPossess;
	
#pragma endregion Abilities
	
#pragma region Input
	
	FGameplayTag InputTag_LMB;
	FGameplayTag Input_Jump;
	FGameplayTag Input_SeekingHost;
	FGameplayTag Input_TryPossess;

#pragma endregion Input
	
#pragma region States

	FGameplayTag State_Parasite_SeekingHost;
	
#pragma endregion States
	
#pragma region Gameplay Cues
	
#pragma region PossessionAbiltyState
	
	FGameplayTag GameplayCue_State_Parasite_SeekingHost;
	FGameplayTag GameplayCue_State_Parasite_FinishSeekingHost;
	
#pragma endregion  PossessionAbiltyState
	
#pragma region Posssesion Result
	
	FGameplayTag GameplayCue_Possession_Captured;
	FGameplayTag GameplayCue_Possession_Fail;
	FGameplayTag GameplayCue_Possession_Pulse;
	FGameplayTag GameplayCue_Possession_Eject_Poop;
	FGameplayTag GameplayCue_Possession_Eject_Vomit;
	
#pragma endregion Posssesion Result
	
#pragma endregion Gameplay Cues

#pragma region Sockets
	FGameplayTag Socket_Butt;
	FGameplayTag Socket_Ear;
	FGameplayTag Socket_Nose;
	FGameplayTag Socket_Mouth;
#pragma endregion Sockets

#pragma region Gameplay Events

	FGameplayTag Event_MiniGame_Start;
	FGameplayTag Event_MiniGame_Finish;
	
#pragma endregion Gameplay Events
	
	static FTBGameplayTags GameplayTags;
};
