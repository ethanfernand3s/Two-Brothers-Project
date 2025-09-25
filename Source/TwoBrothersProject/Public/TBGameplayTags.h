// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/Widget/Inventory/Slots/SlotContainerUserWidget.h"

enum class ECreatureType : uint8;

// TODO: Refactor to namespaces for cleanup/scalability
/**
 * TB Gameplay Tags
 * 
 * Singleton containing native Gameplay Tags
 */
struct FTBGameplayTags
{
	static const FTBGameplayTags& Get() { return GameplayTags;}
	static void InitializeNativeGameplayTags();
	static const FGameplayTag& GetSlotInputTag(FGameplayTag ItemCategory, int32 SlotTileIndex);

	static FTBGameplayTags GameplayTags;
	
#pragma region Gameplay Ability System
	
#pragma region Attributes

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
	FGameplayTag Attributes_Meta_Damage;
	
#pragma endregion Attributes

#pragma region Abilities

	FGameplayTag Abilities_Parasite_SeekHost;
	FGameplayTag Abilities_Parasite_TryPossess;

	FGameplayTag Abilities_Attack;
#pragma endregion Abilities
	
#pragma region Input
	
	FGameplayTag Inputs_Abilities_Main;
	FGameplayTag Inputs_Abilities_Default_1;
	FGameplayTag Inputs_Abilities_Default_2;
	FGameplayTag Inputs_Abilities_Default_3;
	FGameplayTag Inputs_Abilities_Default_4;
	FGameplayTag Inputs_Abilities_Passive_1;
	FGameplayTag Inputs_Abilities_Passive_2;
	FGameplayTag Inputs_Abilities_Passive_3;
	FGameplayTag Inputs_Abilities_Passive_4;

#pragma endregion Input
	
#pragma region States

	FGameplayTag State_Parasite_SeekingHost;
	
#pragma endregion States
	
#pragma region Gameplay Cues
	
#pragma region PossessionAbiltyState
	
	FGameplayTag GameplayCue_State_Parasite_SeekingHost;
	FGameplayTag GameplayCue_State_Parasite_FinishSeekingHost;
	
#pragma endregion  PossessionAbiltyState

#pragma region Posssesion Beginning

	FGameplayTag GameplayCue_Possession_BurrowingIn;
	
#pragma endregion Posssesion Beginning
	
#pragma region Posssesion Result
	
	FGameplayTag GameplayCue_Possession_Captured;
	FGameplayTag GameplayCue_Possession_Fail;
	FGameplayTag GameplayCue_Possession_Pulse;
	FGameplayTag GameplayCue_Possession_Eject_Poop;
	FGameplayTag GameplayCue_Possession_Eject_Vomit;
	
#pragma endregion Posssesion Result
	
#pragma endregion Gameplay Cues

#pragma region Gameplay Events

	FGameplayTag Event_MiniGame_Start;
	FGameplayTag Event_MiniGame_Finish;

#pragma endregion Gameplay Events

#pragma endregion Gameplay Ability System
	
#pragma region BodyParts
	
	FGameplayTag BodyParts_Butt;
	FGameplayTag BodyParts_Ear;
	FGameplayTag BodyParts_Nose;
	FGameplayTag BodyParts_Mouth;
	
#pragma endregion BodyParts

#pragma region Creature Types

	FGameplayTag CreatureTypes_Parasite;
	FGameplayTag CreatureTypes_Earth;
	FGameplayTag CreatureTypes_Wind;
	FGameplayTag CreatureTypes_Fire;
	FGameplayTag CreatureTypes_Water;
	FGameplayTag CreatureTypes_Spirit;
	
#pragma endregion Creature Types

#pragma region Rarities

	FGameplayTag Rarities_Common;
	FGameplayTag Rarities_Uncommon;
	FGameplayTag Rarities_Rare;
	FGameplayTag Rarities_Epic;
	FGameplayTag Rarities_Legendary;
	FGameplayTag Rarities_Prismatic;
	
#pragma endregion Rarities

#pragma region Genders

	FGameplayTag Genders_None;
	FGameplayTag Genders_Male;
	FGameplayTag Genders_Female;
	
#pragma endregion Genders
	
#pragma region Inventory

#pragma region Items

	/* Combination of the seekhost and trypossess ! see trello ! */
	FGameplayTag Items_Abilities_Defaults_Possess;

	FGameplayTag Items_Craftables_TEST;
	
#pragma endregion Items

#pragma region ItemCategories

	FGameplayTag ItemCategories_None;
	FGameplayTag ItemCategories_Abilities_Main;
	FGameplayTag ItemCategories_Abilities_Default;
	FGameplayTag ItemCategories_Abilities_Passive;
	FGameplayTag ItemCategories_BodyPart;
	/* Could add a FGameplayTag ItemCategories_Equipment; // If we need or wanted later on. */
	
#pragma endregion ItemCategories

#pragma region Status

	// The purpose of these are to disable/enable equipping and enabling items
	// (and minor UI display with a lock and tint being applied to the item).
	// Ex. If abilities are added to inventory but are locked then don't add to ASC
	// Else if they're added to inventory and unlocked then add to ASC
	// Else they are equipped so now activate ability
	
	/* Equipped */
	FGameplayTag Status_Equipped;

	/* In the inventory but not equipped */
	FGameplayTag Status_Unlocked;

	/* In the inventory but can't be equipped */
	FGameplayTag Status_Locked;

	// Not in the inventory, on the ground
	FGameplayTag Status_None;
	
#pragma endregion Status
	
#pragma region Fragments

	/* Required for all items */
	FGameplayTag Fragments_Material_Icon;
	
	FGameplayTag Fragments_Text_Name;
	FGameplayTag Fragments_Text_Description;
	
	FGameplayTag Fragments_Rarity;

	/* Additionally required for ability items */
	FGameplayTag Fragments_Ability;
	FGameplayTag Fragments_Status;
	FGameplayTag Fragments_Level;

	/* Optional fragments */
	FGameplayTag Fragments_CreatureType;
	FGameplayTag Fragments_Requirements;
	FGameplayTag Fragments_Input;
	FGameplayTag Fragments_Stackable;
	
#pragma endregion Fragments

#pragma endregion Inventory
};
