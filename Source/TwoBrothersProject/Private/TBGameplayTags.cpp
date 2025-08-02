#include "TBGameplayTags.h"

#include "GameplayCueManager.h"
#include "GameplayTagsManager.h"
#include "AbilitySystem/Data/CreatureType.h"

FTBGameplayTags FTBGameplayTags::GameplayTags;

void FTBGameplayTags::InitializeNativeGameplayTags()
{
#pragma region Attributes

	// Handled manually
	GameplayTags.Attributes_Type = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Type"),
		FString("Current creature type"));

	// Handled by AttributeInfo
	GameplayTags.Attributes_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Health"),
		FString("Current health value")
	);

	GameplayTags.Attributes_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MaxHealth"),
		FString("Maximum health value")
	);

	GameplayTags.Attributes_Defense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Defense"),
		FString("Reduces incoming physical damage")
	);

	GameplayTags.Attributes_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Strength"),
		FString("Affects melee physical damage")
	);

	GameplayTags.Attributes_AuraDefense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.AuraDefense"),
		FString("Reduces incoming aura damage")
	);

	GameplayTags.Attributes_AuraStrength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.AuraStrength"),
		FString("Reduces incoming aura damage")
	);
	
	GameplayTags.Attributes_Speed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Speed"),
		FString("Affects movement speed")
	);
	
	GameplayTags.Attributes_Energy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Energy"),
		FString("Current energy level")
	);

	GameplayTags.Attributes_MaxEnergy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MaxEnergy"),
		FString("Maximum energy level")
	);

	GameplayTags.Attributes_Oxygen = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Oxygen"),
		FString("Current oxygen level")
	);

	GameplayTags.Attributes_MaxOxygen = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MaxOxygen"),
		FString("Maximum oxygen level")
	);

	GameplayTags.Attributes_Drowsiness = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Drowsiness"),
		FString("Current drowsiness value")
		);

	GameplayTags.Attributes_MaxDrowsiness = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MaxDrowsiness"),
		FString("Maximum drowsiness level")
		);

	// Meta
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"),
		FString("Incoming XP Meta Attribute"));
#pragma endregion Attributes

#pragma region Abilities

	GameplayTags.Abilities_Parasite_SeekHost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Parasite.SeekHost"),
		FString("Ability for seeking a host")
	);
	GameplayTags.Abilities_Parasite_TryPossess = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Parasite.TryPossess"),
		FString("Ability for trying to possess a host")
	);

	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Equipped"),
		FString("Ability status when equipped")
	);
	
#pragma endregion Abilities
	
#pragma region Input

	GameplayTags.Inputs_Abilities_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.Abilities.LMB"),
		FString("Input tag for the left mouse button")
	);

	GameplayTags.Inputs_Abilities_Slot1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.Abilities.Slot1"),
		FString("Input tag for the first ability slot")
	);

	GameplayTags.Inputs_Abilities_Slot2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.Abilities.Slot2"),
		FString("Input tag for the second ability slot")
	);

	GameplayTags.Inputs_Abilities_Slot3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.Abilities.Slot3"),
		FString("Input tag for the third ability slot")
		);

	GameplayTags.Inputs_Abilities_Slot4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.Abilities.Slot4"),
		FString("Input tag for the fourth ability slot")
		);

#pragma endregion Input

#pragma region States

	GameplayTags.State_Parasite_SeekingHost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Parasite.SeekingHost"),
		FString("State for seeking a new inhabitant"));
	
#pragma endregion States

#pragma region Gameplay Cues
	
#pragma region PossessionAbiltyState
	
	GameplayTags.GameplayCue_State_Parasite_SeekingHost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.State.Parasite.SeekingHost"),
		FString("Gameplay Cue when starting to seek a new inhabitant"));

	GameplayTags.GameplayCue_State_Parasite_FinishSeekingHost = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.State.Parasite.ExitSeekingHost"),
	FString("Gameplay Cue when finished seeking a new inhabitant"));
	
#pragma endregion PossessionAbiltyState

#pragma region Posssesion Result
	
	GameplayTags.GameplayCue_Possession_Captured = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Possession.Captured"),
		FString("Gameplay Cue when Possession Captured"));

	GameplayTags.GameplayCue_Possession_Fail = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Possession.Fail"),
		FString("Gameplay Cue When Possession Fail"));

	GameplayTags.GameplayCue_Possession_Pulse = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Possession.Pulse"),
			FString("Gameplay Cue for possesion Catch for result"));

	GameplayTags.GameplayCue_Possession_Eject_Poop = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Possession.Eject.Poop"),
		FString("GameplayCue for rejection of possession from the butt"));
	GameplayTags.GameplayCue_Possession_Eject_Vomit = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Possession.Eject.Vomit"),
		FString("GameplayCue for rejection of possession from the head"));
	
#pragma endregion Posssesion Result
	
#pragma endregion Gameplay Cues
	
#pragma region Sockets
	GameplayTags.Sockets_Butt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Sockets.Butt"),
		FString("Gameplay tag for representing the butt socket"));

	GameplayTags.Sockets_Ear = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Sockets.Ear"),
		FString("Gameplay tag for representing the ear socket"));

	GameplayTags.Sockets_Nose = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Sockets.Nose"),
		FString("Gameplay tag for representing the nose socket"));

	GameplayTags.Sockets_Mouth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Sockets.Mouth"),
		FString("Gameplay tag for representing the mouth socket"));
#pragma endregion Sockets
	


#pragma region Gameplay Events

	GameplayTags.Event_MiniGame_Start = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.MiniGame.Start"),
		FString("Event that signifies the start of a minigame."));
	GameplayTags.Event_MiniGame_Finish = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.MiniGame.Finish"),
		FString("Event that signifies the end of a minigame."));
	GameplayTags.GameplayCue_Possession_BurrowingIn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Possession.BurrowingIn"),FString("gameplayCue for burrowing in Animal"));
#pragma endregion Gameplay Events

#pragma region Creature Types

	GameplayTags.CreatureTypes_Parasite = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CreatureTypes.Parasite"),
		FString("Creature type of Parasite"));
	GameplayTags.CreatureTypes_Earth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CreatureTypes.Earth"),
		FString("Creature type of Earth"));
	GameplayTags.CreatureTypes_Wind = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CreatureTypes.Wind"),
		FString("Creature type of Wind"));
	GameplayTags.CreatureTypes_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CreatureTypes.Fire"),
		FString("Creature type of Fire"));
	GameplayTags.CreatureTypes_Water = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CreatureTypes.Water"),
		FString("Creature type of Water"));
	GameplayTags.CreatureTypes_Spirit = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CreatureTypes.Spirit"),
		FString("Creature type of Spirit"));
	
#pragma endregion Creature Types
}

FGameplayTag FTBGameplayTags::EnumToTag(ECreatureType Type)
{
	const auto& Tags = FTBGameplayTags::Get();
	switch (Type)
	{
		case ECreatureType::Parasite: return Tags.CreatureTypes_Parasite;
		case ECreatureType::Wind:     return Tags.CreatureTypes_Wind;
		case ECreatureType::Earth:    return Tags.CreatureTypes_Earth;
		case ECreatureType::Fire:     return Tags.CreatureTypes_Fire;
		case ECreatureType::Water:    return Tags.CreatureTypes_Water;
		case ECreatureType::Spirit:   return Tags.CreatureTypes_Spirit;
		default:                      return FGameplayTag();
	}
}

ECreatureType FTBGameplayTags::TagToEnum(FGameplayTag TypeTag)
{
	static const TMap<FGameplayTag, ECreatureType> TagToEnumMap = {
		{ FTBGameplayTags::Get().CreatureTypes_Parasite , ECreatureType::Parasite },
		{ FTBGameplayTags::Get().CreatureTypes_Wind     , ECreatureType::Wind     },
		{ FTBGameplayTags::Get().CreatureTypes_Earth    , ECreatureType::Earth    },
		{ FTBGameplayTags::Get().CreatureTypes_Fire     , ECreatureType::Fire     },
		{ FTBGameplayTags::Get().CreatureTypes_Water    , ECreatureType::Water    },
		{ FTBGameplayTags::Get().CreatureTypes_Spirit   , ECreatureType::Spirit   }
	};

	if (const ECreatureType* Found = TagToEnumMap.Find(TypeTag))
	{
		return *Found;              
	}

	return ECreatureType::None;
}

int32 FTBGameplayTags::ToSlotIndex(const FGameplayTag& Tag)
{
	const FTBGameplayTags& Tags = FTBGameplayTags::Get();

	static const TMap<FGameplayTag, int32> TagToIndexMap = {
		{ Tags.Inputs_Abilities_LMB , 0 },
		{ Tags.Inputs_Abilities_Slot1, 1 },
		{ Tags.Inputs_Abilities_Slot2, 2 },
		{ Tags.Inputs_Abilities_Slot3, 3 },
		{ Tags.Inputs_Abilities_Slot4, 4 }
	};

	if (const int32* Found = TagToIndexMap.Find(Tag))
	{
		return *Found;
	}
	return INDEX_NONE;
}

FGameplayTag FTBGameplayTags::ToInputTag(int32 SlotIndex)
{
	const FTBGameplayTags& Tags = FTBGameplayTags::Get();

	switch (SlotIndex)
	{
	case 0: return Tags.Inputs_Abilities_LMB;
	case 1: return Tags.Inputs_Abilities_Slot1;
	case 2: return Tags.Inputs_Abilities_Slot2;
	case 3: return Tags.Inputs_Abilities_Slot3;
	case 4: return Tags.Inputs_Abilities_Slot4;
	default: return FGameplayTag();
	}
}