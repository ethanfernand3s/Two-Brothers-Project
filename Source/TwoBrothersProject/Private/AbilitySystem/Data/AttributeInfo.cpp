#include "AbilitySystem/Data/AttributeInfo.h"
#include "TBGameplayTags.h"

FTBAttributeInfo AttributeInfoLibrary::FindAttributeInfo(const FGameplayTag& Tag)
{
	const FTBAttributeInfo* Found = GetAttributeMap().Find(Tag);
	checkf(Found, TEXT("AttributeInfoLibrary::FindAttributeInfo: No info found for tag [%s]"), *Tag.ToString());
	return *Found;
}

const TMap<FGameplayTag, FTBAttributeInfo>& AttributeInfoLibrary::GetAttributeMap()
{
	const FTBGameplayTags& GameplayTags =  FTBGameplayTags::Get();
	static const TMap<FGameplayTag, FTBAttributeInfo> Map = {
		{ GameplayTags.Attributes_Health, FTBAttributeInfo(GameplayTags.Attributes_Health, FText::FromString("Health"), FText::FromString("Current health value"), 0.f) },
		{ GameplayTags.Attributes_MaxHealth, FTBAttributeInfo(GameplayTags.Attributes_MaxHealth, FText::FromString("Max Health"), FText::FromString("Maximum health value"), 0.f) },
		{ GameplayTags.Attributes_Defense, FTBAttributeInfo(GameplayTags.Attributes_Defense, FText::FromString("Defense"), FText::FromString("Reduces incoming physical damage"), 0.f) },
		{ GameplayTags.Attributes_Strength, FTBAttributeInfo(GameplayTags.Attributes_Strength, FText::FromString("Strength"), FText::FromString("Affects melee physical damage"), 0.f) },
		{ GameplayTags.Attributes_AuraDefense, FTBAttributeInfo(GameplayTags.Attributes_AuraDefense, FText::FromString("AuraDefense"), FText::FromString("Reduces incoming aura damage"), 0.f) },
		{ GameplayTags.Attributes_AuraStrength, FTBAttributeInfo(GameplayTags.Attributes_AuraStrength, FText::FromString("AuraStrength"), FText::FromString("Affects melee aura damage"), 0.f) },
		{ GameplayTags.Attributes_Speed, FTBAttributeInfo(GameplayTags.Attributes_Speed, FText::FromString("Speed"), FText::FromString("Affects movement speed"), 0.f) },
		{ GameplayTags.Attributes_Energy, FTBAttributeInfo(GameplayTags.Attributes_Energy, FText::FromString("Energy"), FText::FromString("Current energy level"), 0.f) },
		{ GameplayTags.Attributes_MaxEnergy, FTBAttributeInfo(GameplayTags.Attributes_MaxEnergy, FText::FromString("Max Energy"), FText::FromString("Maximum energy level"), 0.f) },
		{ GameplayTags.Attributes_Oxygen, FTBAttributeInfo(GameplayTags.Attributes_Oxygen, FText::FromString("Oxygen"), FText::FromString("Current oxygen level"), 0.f) },
		{ GameplayTags.Attributes_MaxOxygen, FTBAttributeInfo(GameplayTags.Attributes_MaxOxygen, FText::FromString("Max Oxygen"), FText::FromString("Maximum oxygen level"), 0.f) },
		{ GameplayTags.Attributes_Drowsiness, FTBAttributeInfo(GameplayTags.Attributes_Drowsiness, FText::FromString("Drowsiness"), FText::FromString("Current drowsiness value"), 0.f) },
		{ GameplayTags.Attributes_MaxDrowsiness, FTBAttributeInfo(GameplayTags.Attributes_MaxDrowsiness, FText::FromString("MaxDrowsiness"), FText::FromString("Maximum drowsiness value"), 0.f) },
	};
	return Map;
}