#include "AbilitySystem/Data/AttributeInfo.h"
#include "TBGameplayTags.h"

FTBAttributeInfo AttributeInfoLibrary::FindAttributeInfo(const FGameplayTag& Tag)
{
	const FTBAttributeInfo* Found = GetAttributeMap().Find(Tag);
	checkf(Found, TEXT("AttributeInfoLibrary::FindFloatAttributeInfo: No info found for tag [%s]"), *Tag.ToString());
	return *Found;
}

const TMap<FGameplayTag, FTBAttributeInfo>& AttributeInfoLibrary::GetAttributeMap()
{
	const FTBGameplayTags& GameplayTags =  FTBGameplayTags::Get();
	static const TMap<FGameplayTag, FTBAttributeInfo> Map = {
		{ GameplayTags.Attributes_Type, FTBAttributeInfo(GameplayTags.Attributes_Type, FText::FromString("Type"), FText::FromString("Current creature type"), 0.f) },
		{ GameplayTags.Attributes_Health, FTBAttributeInfo(GameplayTags.Attributes_Health, FText::FromString("Health"), FText::FromString("Current health value"), 0.f) },
		{ GameplayTags.Attributes_MaxHealth, FTBAttributeInfo(GameplayTags.Attributes_MaxHealth, FText::FromString("Max Health"), FText::FromString("Maximum health value"), 0.f) },
		{ GameplayTags.Attributes_Defense, FTBAttributeInfo(GameplayTags.Attributes_Defense, FText::FromString("Defense"), FText::FromString("Reduces incoming damage"), 0.f) },
		{ GameplayTags.Attributes_Thirst, FTBAttributeInfo(GameplayTags.Attributes_Thirst, FText::FromString("Thirst"), FText::FromString("Current thirst level"), 0.f) },
		{ GameplayTags.Attributes_MaxThirst, FTBAttributeInfo(GameplayTags.Attributes_MaxThirst, FText::FromString("Max Thirst"), FText::FromString("Maximum thirst level"), 0.f) },
		{ GameplayTags.Attributes_Hunger, FTBAttributeInfo(GameplayTags.Attributes_Hunger, FText::FromString("Hunger"), FText::FromString("Current hunger level"), 0.f) },
		{ GameplayTags.Attributes_MaxHunger, FTBAttributeInfo(GameplayTags.Attributes_MaxHunger, FText::FromString("Max Hunger"), FText::FromString("Maximum hunger level"), 0.f) },
		{ GameplayTags.Attributes_Stamina, FTBAttributeInfo(GameplayTags.Attributes_Stamina, FText::FromString("Stamina"), FText::FromString("Current stamina level"), 0.f) },
		{ GameplayTags.Attributes_MaxStamina, FTBAttributeInfo(GameplayTags.Attributes_MaxStamina, FText::FromString("Max Stamina"), FText::FromString("Maximum stamina level"), 0.f) },
		{ GameplayTags.Attributes_Oxygen, FTBAttributeInfo(GameplayTags.Attributes_Oxygen, FText::FromString("Oxygen"), FText::FromString("Current oxygen level"), 0.f) },
		{ GameplayTags.Attributes_MaxOxygen, FTBAttributeInfo(GameplayTags.Attributes_MaxOxygen, FText::FromString("Max Oxygen"), FText::FromString("Maximum oxygen level"), 0.f) },
		{ GameplayTags.Attributes_Strength, FTBAttributeInfo(GameplayTags.Attributes_Strength, FText::FromString("Strength"), FText::FromString("Affects melee damage"), 0.f) },
		{ GameplayTags.Attributes_Speed, FTBAttributeInfo(GameplayTags.Attributes_Speed, FText::FromString("Speed"), FText::FromString("Affects movement speed"), 0.f) },
		{ GameplayTags.Attributes_Drowsiness, FTBAttributeInfo(GameplayTags.Attributes_Drowsiness, FText::FromString("Drowsiness"), FText::FromString("Current drowsiness value"), 0.f) },
		{ GameplayTags.Attributes_MaxDrowsiness, FTBAttributeInfo(GameplayTags.Attributes_MaxDrowsiness, FText::FromString("MaxDrowsiness"), FText::FromString("Maximum drowsiness value"), 0.f) },
	};
	return Map;
}