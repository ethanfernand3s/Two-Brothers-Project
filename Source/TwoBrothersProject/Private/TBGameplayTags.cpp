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
		FString("Reduces incoming damage")
	);

	GameplayTags.Attributes_Thirst = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Thirst"),
		FString("Current thirst level")
	);

	GameplayTags.Attributes_MaxThirst = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MaxThirst"),
		FString("Maximum thirst level")
	);

	GameplayTags.Attributes_Hunger = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Hunger"),
		FString("Current hunger level")
	);

	GameplayTags.Attributes_MaxHunger = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MaxHunger"),
		FString("Maximum hunger level")
	);

	GameplayTags.Attributes_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Stamina"),
		FString("Current stamina level")
	);

	GameplayTags.Attributes_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MaxStamina"),
		FString("Maximum stamina level")
	);

	GameplayTags.Attributes_Oxygen = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Oxygen"),
		FString("Current oxygen level")
	);

	GameplayTags.Attributes_MaxOxygen = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MaxOxygen"),
		FString("Maximum oxygen level")
	);

	GameplayTags.Attributes_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Strength"),
		FString("Affects melee damage")
	);

	GameplayTags.Attributes_Speed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Speed"),
		FString("Affects movement speed")
	);

	GameplayTags.Attributes_TemperatureResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.TemperatureResistance"),
		FString("Affects resistance to temperatures effects")
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

	GameplayTags.GameplayCue_State_Parasite_SeekingHost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.State.Parasite.SeekingHost"),
		FString("Gameplay Cue when starting to seek a new inhabitant"));

	GameplayTags.GameplayCue_State_Parasite_FinishSeekingHost = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.State.Parasite.ExitSeekingHost"),
	FString("Gameplay Cue when finished seeking a new inhabitant"));
	
#pragma endregion Gameplay Cues
}
