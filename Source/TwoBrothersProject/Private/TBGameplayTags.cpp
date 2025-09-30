#include "TBGameplayTags.h"

#include "GameplayCueManager.h"
#include "GameplayTagsManager.h"

FTBGameplayTags FTBGameplayTags::GameplayTags;

void FTBGameplayTags::InitializeNativeGameplayTags()
{

#pragma region Gameplay Ability System
	
#pragma region Attributes

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
	GameplayTags.Attributes_Meta_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.Damage"),
		FString("Gameplay tag for incoming damage meta attribute"));
	
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
	
#pragma endregion Abilities
	
#pragma region Input

	GameplayTags.Inputs_Abilities_Main = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.Abilities.Main"),
		FString("Input tag for the main ability")
	);
	GameplayTags.Inputs_Abilities_Default_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.Abilities.Default.1"),
		FString("Input tag for the default ability in slot 1")
	);
	GameplayTags.Inputs_Abilities_Default_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.Abilities.Default.2"),
		FString("Input tag for the default ability in slot 2")
	);
	GameplayTags.Inputs_Abilities_Default_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.Abilities.Default.3"),
		FString("Input tag for the default ability in slot 3")
	);
	GameplayTags.Inputs_Abilities_Default_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.Abilities.Default.4"),
		FString("Input tag for the default ability in slot 4")
	);
	GameplayTags.Inputs_Abilities_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.Abilities.Passive.1"),
		FString("Input tag for the passive ability in slot 1")
	);
	GameplayTags.Inputs_Abilities_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.Abilities.Passive.2"),
		FString("Input tag for the passive ability in slot 2")
	);
	GameplayTags.Inputs_Abilities_Passive_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.Abilities.Passive.3"),
		FString("Input tag for the passive ability in slot 3")
	);
	GameplayTags.Inputs_Abilities_Passive_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.Abilities.Passive.4"),
		FString("Input tag for the passive ability in slot 4")
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

#pragma region Possession Begin

	GameplayTags.GameplayCue_Possession_BurrowingIn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Possession.BurrowingIn"),FString("gameplayCue for burrowing in Animal"));
	
#pragma endregion Possession Begin
	
#pragma region Possession Result
	
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
	
#pragma endregion Possession Result
	
#pragma endregion Gameplay Cues

#pragma region Gameplay Events

	GameplayTags.Event_MiniGame_Start = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.MiniGame.Start"),
		FString("Event that signifies the start of a minigame."));
	GameplayTags.Event_MiniGame_Finish = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.MiniGame.Finish"),
		FString("Event that signifies the end of a minigame."));
	
#pragma endregion Gameplay Events

#pragma endregion Gameplay Ability System
	
#pragma region BodyParts
	
	GameplayTags.BodyParts_Butt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("BodyParts.Butt"),
		FString("Gameplay tag for representing the butt body part"));

	GameplayTags.BodyParts_Ear = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("BodyParts.Ear"),
		FString("Gameplay tag for representing the ear body part"));

	GameplayTags.BodyParts_Nose = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("BodyParts.Nose"),
		FString("Gameplay tag for representing the nose body part"));

	GameplayTags.BodyParts_Mouth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("BodyParts.Mouth"),
		FString("Gameplay tag for representing the mouth body part"));
	
#pragma endregion BodyParts

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

#pragma region Rarities

	GameplayTags.Rarities_Common = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Rarities.Common"),
		FString("Gameplay tag the common rarity"));
	GameplayTags.Rarities_Uncommon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Rarities.Uncommon"),
	FString("Gameplay tag the uncommon rarity"));
	GameplayTags.Rarities_Rare = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Rarities.Rare"),
	FString("Gameplay tag the rare rarity"));
	GameplayTags.Rarities_Epic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Rarities.Epic"),
	FString("Gameplay tag the epic rarity"));
	GameplayTags.Rarities_Legendary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Rarities.Legendary"),
	FString("Gameplay tag the legendary rarity"));
	GameplayTags.Rarities_Prismatic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Rarities.Prismatic"),
	FString("Gameplay tag the prismatic rarity"));
	
#pragma endregion Rarities

#pragma region Genders

	GameplayTags.Genders_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Genders.None"),
		FString("Gameplay tag no gender"));
	GameplayTags.Genders_Male = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Genders.Male"),
		FString("Gameplay tag the male gender"));
	GameplayTags.Genders_Female = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Genders.Female"),
		FString("Gameplay tag the female gender"));
	
#pragma endregion Genders
	
#pragma region Inventory
	
#pragma region Items

	GameplayTags.Items_Abilities_Defaults_Possess = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Items.Abilities.Defaults.Possess"),
	FString("This ability allows the user to launch forward. "
		 "If a hole in a potential inhabitants body is hit the user has the chance to possess."));

	GameplayTags.Items_Abilities_Defaults_SeekHost = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Items.Abilities.Defaults.SeekHost"),
	FString("This ability allows the user to enter a hunting mode showing nearby inhabitants."));
	
	GameplayTags.Items_AbilityCardholder = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Items.AbilityCardholder"),
	FString("Used for crafting an ability card."));
	
#pragma endregion Items

#pragma region ItemCategories

	GameplayTags.ItemCategories_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("ItemCategories.None"),
	FString("No item category"));
	GameplayTags.ItemCategories_Abilities_Main = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("ItemCategories.Abilities.Main"),
	FString("Main ability item category"));
	GameplayTags.ItemCategories_Abilities_Default = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("ItemCategories.Abilities.Default"),
	FString("Default ability item category"));
	GameplayTags.ItemCategories_Abilities_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("ItemCategories.Abilities.Passive"),
	FString("Passive ability item category"));
	GameplayTags.ItemCategories_BodyPart = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("ItemCategories.BodyPart"),
	FString("BodyPart item category"));
	
#pragma endregion ItemCategories
	
#pragma region Status

	GameplayTags.Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Equipped"),
		FString("In the inventory and equipped."));
	
	GameplayTags.Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Unlocked"),
		FString("In the inventory but not equipped."));
	
	GameplayTags.Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Locked"),
		FString("In the inventory but can't be equipped."));

	GameplayTags.Status_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.None"),
		FString("Not in the inventory, on the ground."));
	
#pragma endregion Status
	
#pragma region Fragments

	/* Required for all items*/
	GameplayTags.Fragments_Material_Icon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Fragments.Material.Icon"),
		FString("An Inventory items fragment holding the data needed to display an icon"));
	
	GameplayTags.Fragments_Text_Name = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Fragments.Text.Name"),
		FString("An Inventory items fragment holding the data needed to display its name"));
	GameplayTags.Fragments_Text_Description = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Fragments.Text.Description"),
		FString("An Inventory items fragment holding the data needed to display its description"));
	
	GameplayTags.Fragments_Rarity = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Fragments.Rarity"),
		FString("An Inventory items fragment holding the data needed to display its rarity"));

	/* Additionally required for ability items */
	GameplayTags.Fragments_Ability = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Fragments.Ability"),
			FString("An Inventory items fragment holding the data needed to display an ability"));
	GameplayTags.Fragments_Status = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Fragments.Status"),
			FString("An Inventory items fragment holding the data need to display an item's status"));
	GameplayTags.Fragments_Level = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Fragments.Level"),
			FString("An Inventory items fragment holding the data need to display an item's level"));

	/* Optional fragments */
	GameplayTags.Fragments_CreatureType = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Fragments.CreatureType"),
			FString("An Inventory items fragment holding the data need to display an item's creature type"));
	GameplayTags.Fragments_Requirements = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Fragments.Requirements"),
			FString("An Inventory items fragment holding the data need to display an item's requirements to be used"));
	GameplayTags.Fragments_Input = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Fragments.Input"),
			FString("An Inventory items fragment holding the data needed to bind input"));
	GameplayTags.Fragments_Stackable = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Fragments.Stackable"),
			FString("An Inventory items fragment holding the data needed to stack an item"));
	
#pragma endregion Fragments
	
#pragma endregion Inventory
}

const FGameplayTag& FTBGameplayTags::GetSlotInputTag(FGameplayTag ItemCategory, int32 SlotTileIndex)
{
	auto& Tags = Get();

	// Main ability only has one slot → always return Main
	if (ItemCategory.MatchesTagExact(Tags.ItemCategories_Abilities_Main))
	{
		return Tags.Inputs_Abilities_Main;
	}

	// Default ability slots
	if (ItemCategory.MatchesTagExact(Tags.ItemCategories_Abilities_Default))
	{
		switch (SlotTileIndex)
		{
		case 0: return Tags.Inputs_Abilities_Default_1;
		case 1: return Tags.Inputs_Abilities_Default_2;
		case 2: return Tags.Inputs_Abilities_Default_3;
		case 3: return Tags.Inputs_Abilities_Default_4;
		default: break;
		}
	}

	// Passive ability slots
	if (ItemCategory.MatchesTagExact(Tags.ItemCategories_Abilities_Passive))
	{
		switch (SlotTileIndex)
		{
		case 0: return Tags.Inputs_Abilities_Passive_1;
		case 1: return Tags.Inputs_Abilities_Passive_2;
		case 2: return Tags.Inputs_Abilities_Passive_3;
		case 3: return Tags.Inputs_Abilities_Passive_4;
		default: break;
		}
	}

	// Fallback
	return FGameplayTag::EmptyTag;
}
