#include "Characters/CharacterContextComponent.h"

#include "Characters/Data/BiomeDataAsset.h"
#include "Characters/Data/Gender.h"
#include "Net/UnrealNetwork.h"

UCharacterContextComponent::UCharacterContextComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Gender = ECharacterGender::None;
	SetIsReplicatedByDefault(true);
}

void UCharacterContextComponent::InitializeCharacterContext(const FText& InName,
		const int32 InLevel,
		const int32 InXP,
		const FTribeData& InTribeData,
		const ECharacterGender InGender,
		const int32 InAttributePoints,
		const ERarity InRarity,
		const float InWeightedBST,
		const EGrowthRate InGrowthRate)
{
	SetCharacterName(InName);
	SetLevel(InLevel);
	SetXP(InXP);
	SetTribeData(InTribeData);
	SetGender(InGender);
	SetAttributePoints(InAttributePoints);
	SetRarity(InRarity);
	SetGrowthRate(InGrowthRate);

	InitializeRandomIVs();
	InitializeBaseXP(InWeightedBST);


	OnCharacterNameChanged.Broadcast(CharacterName);
	OnLevelChanged.Broadcast(Level);
	OnXPChanged.Broadcast(XP);
	OnTribeDataChanged.Broadcast(TribeData);
	if (BiomeData != nullptr)
	{
		OnBiomeChanged.Broadcast(BiomeData->BiomeInfo);
	}
	
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

void UCharacterContextComponent::InitializeBaseXP(const float WeightedBST)
{
	float RarityMultiplier{0.f};
	switch (Rarity)
	{
		case ERarity::Common:
			RarityMultiplier = .18f;
		case ERarity::Uncommon:
			RarityMultiplier = .22f;
	case ERarity::Rare:
		RarityMultiplier = .31f;
	case ERarity::Epic:
		RarityMultiplier = .40f;
	case ERarity::Legendary:
		RarityMultiplier = .50f;
	case ERarity::Mythical:
		RarityMultiplier = .60f;
	default:
		RarityMultiplier = .18f;
	}

	BaseXP = WeightedBST * RarityMultiplier;
}

void UCharacterContextComponent::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChanged.Broadcast(XP);
}

void UCharacterContextComponent::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChanged.Broadcast(Level);
}

void UCharacterContextComponent::AddToAttributePoints(int32 InPoints)
{
	AttributePoints += InPoints;
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

void UCharacterContextComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCharacterContextComponent, CharacterName);
	DOREPLIFETIME(UCharacterContextComponent, Level);
	DOREPLIFETIME(UCharacterContextComponent, XP);
	DOREPLIFETIME(UCharacterContextComponent, TribeData);
	DOREPLIFETIME(UCharacterContextComponent, Gender);
	DOREPLIFETIME(UCharacterContextComponent, BiomeData);
	DOREPLIFETIME(UCharacterContextComponent, AttributePoints);
	DOREPLIFETIME(UCharacterContextComponent, Rarity);
	DOREPLIFETIME(UCharacterContextComponent, BaseXP);
	DOREPLIFETIME(UCharacterContextComponent, IVSet);
	DOREPLIFETIME(UCharacterContextComponent, LevelGrowthRate);
}

void UCharacterContextComponent::OnRep_CharacterName()
{
	OnCharacterNameChanged.Broadcast(CharacterName);
}

void UCharacterContextComponent::OnRep_Level()
{
	OnLevelChanged.Broadcast(Level);
}

void UCharacterContextComponent::OnRep_XP()
{
	OnXPChanged.Broadcast(XP);
}

void UCharacterContextComponent::OnRep_TribeData()
{
	OnTribeDataChanged.Broadcast(TribeData);
}

void UCharacterContextComponent::OnRep_BiomeData()
{
	if (BiomeData != nullptr)
	{
		OnBiomeChanged.Broadcast(BiomeData->BiomeInfo);
	}
}

void UCharacterContextComponent::OnRep_AttributePoints()
{
	OnAttributePointsChanged.Broadcast(AttributePoints);
}
