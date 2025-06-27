#include "TBGameplayTags.h"
#include "GameplayTagsManager.h"

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
		FName("Attributes.Defense"),
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

#pragma endregion Attributes

#pragma region Abilities

	GameplayTags.Ability_Parasite_SeekHost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Parasite.SeekHost"),
		FString("Ability for seeking a host")
	);
	GameplayTags.Ability_Parasite_TryPossess = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Parasite.TryPossess"),
		FString("Ability for trying to possess a host")
	);
	
#pragma endregion Abilities
	
#pragma region Input

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button")
	);

	GameplayTags.Input_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Jump"),
		FString("Input Tag Associated for the Jump")
	);

	GameplayTags.Input_SeekingHost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.SeekingHost"),
		FString("Input Tag Associated for the EnterSeekingHostAbility")
	);

	GameplayTags.Input_TryPossess = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.TryPossess"),
		FString("Input Tag Associated for the PossessAbility")
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

#pragma region Sockets
	GameplayTags.Socket_Butt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Socket.Butt"),
		FString("Gameplay tag for representing the butt socket"));

	GameplayTags.Socket_Ear = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Socket.Ear"),
		FString("Gameplay tag for representing the ear socket"));

	GameplayTags.Socket_Nose = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Socket.Nose"),
		FString("Gameplay tag for representing the nose socket"));

	GameplayTags.Socket_Mouth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Socket.Mouth"),
		FString("Gameplay tag for representing the mouth socket"));
#pragma endregion Sockets
	
#pragma endregion Gameplay Cues

#pragma region Gameplay Events

	GameplayTags.Event_MiniGame_Start = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.MiniGame.Start"),
		FString("Event that signifies the start of a minigame."));
	GameplayTags.Event_MiniGame_Finish = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.MiniGame.Finish"),
		FString("Event that signifies the end of a minigame."));
	
#pragma endregion Gameplay Events
}
