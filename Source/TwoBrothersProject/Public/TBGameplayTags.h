// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

enum class ECreatureType : uint8;
/**
 * TB Gameplay Tags
 * 
 * Singleton containing native Gameplay Tags
 */
struct FTBGameplayTags
{

	// Can be overloaded with any other enum -> tags
	static FGameplayTag EnumToTag(ECreatureType Type);
	static ECreatureType TagToEnum(FGameplayTag TypeTag);
	
	static int32 ToSlotIndex(const FGameplayTag& Tag);
	static FGameplayTag ToInputTag(int32 SlotIndex);
	
	static const FTBGameplayTags& Get() { return GameplayTags;}
	static void InitializeNativeGameplayTags();
	
#pragma region Attributes

	//Creature Type
	FGameplayTag Attributes_Type;

	// Double
	FGameplayTag Attributes_Health;
	FGameplayTag Attributes_MaxHealth;

	FGameplayTag Attributes_Energy;
	FGameplayTag Attributes_MaxEnergy;

	FGameplayTag Attributes_Oxygen;
	FGameplayTag Attributes_MaxOxygen;

	FGameplayTag Attributes_Drowsiness;
	FGameplayTag Attributes_MaxDrowsiness;

	// Single 
	FGameplayTag Attributes_Defense;
	FGameplayTag Attributes_Strength;
	FGameplayTag Attributes_AuraDefense;
	FGameplayTag Attributes_AuraStrength;
	FGameplayTag Attributes_Speed;

	// Meta
	FGameplayTag Attributes_Meta_IncomingXP;
	
#pragma endregion Attributes

#pragma region Abilities

	FGameplayTag Abilities_Parasite_SeekHost;
	FGameplayTag Abilities_Parasite_TryPossess;

	FGameplayTag Abilities_Status_Equipped;
	
#pragma endregion Abilities
	
#pragma region Input
	
	FGameplayTag Inputs_Abilities_LMB;
	FGameplayTag Inputs_Abilities_Slot1;
	FGameplayTag Inputs_Abilities_Slot2;
	FGameplayTag Inputs_Abilities_Slot3;
	FGameplayTag Inputs_Abilities_Slot4;

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
	FGameplayTag Sockets_Butt;
	FGameplayTag Sockets_Ear;
	FGameplayTag Sockets_Nose;
	FGameplayTag Sockets_Mouth;
#pragma endregion Sockets

#pragma region Gameplay Events

	FGameplayTag Event_MiniGame_Start;
	FGameplayTag Event_MiniGame_Finish;
	FGameplayTag GameplayCue_Possession_BurrowingIn;

#pragma endregion Gameplay Events

#pragma region Creature Types

	FGameplayTag CreatureTypes_Parasite;
	FGameplayTag CreatureTypes_Earth;
	FGameplayTag CreatureTypes_Wind;
	FGameplayTag CreatureTypes_Fire;
	FGameplayTag CreatureTypes_Water;
	FGameplayTag CreatureTypes_Spirit;
	
#pragma endregion Creature Types
	
	static FTBGameplayTags GameplayTags;
};
