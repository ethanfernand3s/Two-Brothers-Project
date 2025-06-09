#include "AbilitySystem/Data/AttributeInfo.h"
#include "TBGameplayTags.h"

FTBFloatAttributeInfo AttributeInfoLibrary::FindFloatAttributeInfo(const FGameplayTag& Tag)
{
	const FTBFloatAttributeInfo* Found = GetFloatAttributeMap().Find(Tag);
	checkf(Found, TEXT("AttributeInfoLibrary::FindFloatAttributeInfo: No info found for tag [%s]"), *Tag.ToString());
	return *Found;
}

const TMap<FGameplayTag, FTBFloatAttributeInfo>& AttributeInfoLibrary::GetFloatAttributeMap()
{
	static const TMap<FGameplayTag, FTBFloatAttributeInfo> Map = {
		{ FTBGameplayTags::Get().Attributes_Health, FTBFloatAttributeInfo(FText::FromString("Health"), FText::FromString("Current health value"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_MaxHealth, FTBFloatAttributeInfo(FText::FromString("Max Health"), FText::FromString("Maximum health value"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Defense, FTBFloatAttributeInfo(FText::FromString("Defense"), FText::FromString("Reduces incoming damage"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Thirst, FTBFloatAttributeInfo(FText::FromString("Thirst"), FText::FromString("Current thirst level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_MaxThirst, FTBFloatAttributeInfo(FText::FromString("Max Thirst"), FText::FromString("Maximum thirst level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Hunger, FTBFloatAttributeInfo(FText::FromString("Hunger"), FText::FromString("Current hunger level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_MaxHunger, FTBFloatAttributeInfo(FText::FromString("Max Hunger"), FText::FromString("Maximum hunger level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Stamina, FTBFloatAttributeInfo(FText::FromString("Stamina"), FText::FromString("Current stamina level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_MaxStamina, FTBFloatAttributeInfo(FText::FromString("Max Stamina"), FText::FromString("Maximum stamina level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Oxygen, FTBFloatAttributeInfo(FText::FromString("Oxygen"), FText::FromString("Current oxygen level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_MaxOxygen, FTBFloatAttributeInfo(FText::FromString("Max Oxygen"), FText::FromString("Maximum oxygen level"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Strength, FTBFloatAttributeInfo(FText::FromString("Strength"), FText::FromString("Affects melee damage"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_Speed, FTBFloatAttributeInfo(FText::FromString("Speed"), FText::FromString("Affects movement speed"), 0.f) },
		{ FTBGameplayTags::Get().Attributes_TemperatureResistance, FTBFloatAttributeInfo(FText::FromString("Temperature Resistance"), FText::FromString("Resists environmental temperature effects"), 0.f) },
	};
	return Map;
}

FTBCreatureTypeAttributeInfo AttributeInfoLibrary::GetCreatureTypeAttribute()
{
	return FTBCreatureTypeAttributeInfo();
}
