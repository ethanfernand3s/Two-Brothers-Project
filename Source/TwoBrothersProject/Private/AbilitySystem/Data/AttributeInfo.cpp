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
	static const TMap<FGameplayTag, FTBAttributeInfo> Map = {
		{ FTBGameplayTags::Get().Attributes_Type, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_Type, FText::FromString("Type"), FText::FromString("Current creature type"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Health, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_Health, FText::FromString("Health"), FText::FromString("Current health value"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_MaxHealth, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_MaxHealth, FText::FromString("Max Health"), FText::FromString("Maximum health value"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Defense, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_Defense, FText::FromString("Defense"), FText::FromString("Reduces incoming damage"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Thirst, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_Thirst, FText::FromString("Thirst"), FText::FromString("Current thirst level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_MaxThirst, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_MaxThirst, FText::FromString("Max Thirst"), FText::FromString("Maximum thirst level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Hunger, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_Hunger, FText::FromString("Hunger"), FText::FromString("Current hunger level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_MaxHunger, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_MaxHunger, FText::FromString("Max Hunger"), FText::FromString("Maximum hunger level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Stamina, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_Stamina, FText::FromString("Stamina"), FText::FromString("Current stamina level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_MaxStamina, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_MaxStamina, FText::FromString("Max Stamina"), FText::FromString("Maximum stamina level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Oxygen, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_Oxygen, FText::FromString("Oxygen"), FText::FromString("Current oxygen level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_MaxOxygen, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_MaxOxygen, FText::FromString("Max Oxygen"), FText::FromString("Maximum oxygen level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Strength, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_Strength, FText::FromString("Strength"), FText::FromString("Affects melee damage"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Speed, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_Speed, FText::FromString("Speed"), FText::FromString("Affects movement speed"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_TemperatureResistance, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_TemperatureResistance, FText::FromString("Temperature Resistance"), FText::FromString("Resists environmental temperature effects"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Drowsiness, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_Drowsiness, FText::FromString("Drowsiness"), FText::FromString("Current drowsiness value"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_MaxDrowsiness, FTBAttributeInfo(FTBGameplayTags::Get().Attributes_MaxDrowsiness, FText::FromString("MaxDrowsiness"), FText::FromString("Maximum drowsiness value"), 0.f) },
	};
	return Map;
}