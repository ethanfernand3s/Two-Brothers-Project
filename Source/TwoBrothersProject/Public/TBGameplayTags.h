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
	FGameplayTag Attributes_TemperatureResistance;
	
#pragma endregion Attributes
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

	FGameplayTag GameplayCue_State_Parasite_SeekingHost;
	FGameplayTag GameplayCue_State_Parasite_FinishSeekingHost;
	
#pragma endregion Gameplay Cues
private:
	
	static FTBGameplayTags GameplayTags;
};
