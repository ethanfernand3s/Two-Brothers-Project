#include "Characters/CharacterContextComponent.h"

#include "AbilitySystem/Data/CreatureType.h"
#include "Characters/Data/BiomeDataAsset.h"
#include "Characters/Data/Gender.h"
#include "Characters/Data/GrowthRate.h"
#include "Net/UnrealNetwork.h"
#include "Rarity/Data/Rarity.h"

UCharacterContextComponent::UCharacterContextComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Gender = ECharacterGender::None;
	Rarity = ERarity::Common;
	PrimaryType = ECreatureType::None;
	SecondaryType = ECreatureType::None;
	AuraColor = FColor::White;
	SetIsReplicatedByDefault(true);
}

void UCharacterContextComponent::InitializeCharacterContext(
		const FText& InName,
		const int32 InLevel,
		const int32 InXP,
		const FTribeData& InTribeData,
		const ECharacterGender InGender,
		const int32 InAttributePoints
	)
{
	SetCharacterName(InName);
	SetLevel(InLevel);
	SetXP(InXP);
	SetTribeData(InTribeData);
	SetGender(InGender);
	SetAttributePoints(InAttributePoints);
	
	OnCharacterNameChanged.Broadcast(CharacterName);
	OnLevelChanged.Broadcast(Level);
	OnXPChanged.Broadcast(XP);
	OnTribeDataChanged.Broadcast(TribeData);
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

void UCharacterContextComponent::InitializeCombatRelatedVars(float CustomBaseCombatPower)
{
	// Add IV's to BaseCombatPower
	InitializeRandomIVs();
	BaseCombatPower += IVSet.GetTotal();


	// * Temporary Elemental type will be decided based on biome and parts * \\
	
	PrimaryType = static_cast<ECreatureType>(FMath::RandRange(1,6));
	SecondaryType = static_cast<ECreatureType>(FMath::RandRange(0,6));
	
	if (CustomBaseCombatPower == -1.f)
	{
		// === Randomly roll rarity and base CP ===
		float Roll = FMath::FRand();
	
		// Leave the rest to chance
		if (Roll < 0.4f)
		{
			BaseCombatPower = FMath::RandRange(200, 300);
			Rarity = ERarity::Common;
			BaseXP = BaseCombatPower * 0.18f;
			LevelGrowthRate = EGrowthRate::Fast;
			
			AuraColor = FColor::White;
		}
		else if (Roll < 0.65f)
		{
			BaseCombatPower = FMath::RandRange(300, 400);
			Rarity = ERarity::Uncommon;
			BaseXP = BaseCombatPower * 0.22f;
			LevelGrowthRate = EGrowthRate::MediumFast;
			
			AuraColor = FColor::Silver;
		}
		else if (Roll < 0.80f)
		{
			BaseCombatPower = FMath::RandRange(400, 500);
			Rarity = ERarity::Rare;
			BaseXP = BaseCombatPower * 0.31f;
			LevelGrowthRate = EGrowthRate::MediumSlow;
			
			AuraColor = FColor::Blue;
		}
		else if (Roll < 0.95f)
		{
			BaseCombatPower = FMath::RandRange(500, 600);
			Rarity = ERarity::Epic;
			BaseXP = BaseCombatPower * 0.40f;
			LevelGrowthRate = EGrowthRate::Slow;
			
			AuraColor = FColor::Purple;
		}
		else if (Roll < 0.99f)
		{
			BaseCombatPower = FMath::RandRange(650, 750);
			Rarity = ERarity::Legendary;
			BaseXP = BaseCombatPower * 0.50f;
			LevelGrowthRate = EGrowthRate::Fluctuating;
			
			AuraColor = FColor::Orange;
		}
		else
		{
			BaseCombatPower = FMath::RandRange(800, 1000);
			Rarity = ERarity::Mythical;
			BaseXP = BaseCombatPower * 0.60f;
			LevelGrowthRate = EGrowthRate::Erratic;

			AuraColor = FColor::Red;
		}
	}
	else
	{
		// === Use passed-in base CP to determine category ===
		BaseCombatPower = CustomBaseCombatPower;

		if (BaseCombatPower < 300.f)
		{
			Rarity = ERarity::Common;
			BaseXP = BaseCombatPower * 0.18f;
			LevelGrowthRate = EGrowthRate::Fast;
		}
		else if (BaseCombatPower < 400.f)
		{
			Rarity = ERarity::Uncommon;
			BaseXP = BaseCombatPower * 0.22f;
			LevelGrowthRate = EGrowthRate::MediumFast;
		}
		else if (BaseCombatPower < 500.f)
		{
			Rarity = ERarity::Rare;
			BaseXP = BaseCombatPower * 0.31f;
			LevelGrowthRate = EGrowthRate::MediumSlow;
		}
		else if (BaseCombatPower < 650.f)
		{
			Rarity = ERarity::Epic;
			BaseXP = BaseCombatPower * 0.40f;
			LevelGrowthRate = EGrowthRate::Slow;
		}
		else if (BaseCombatPower < 800.f)
		{
			Rarity = ERarity::Legendary;
			BaseXP = BaseCombatPower * 0.50f;
			LevelGrowthRate = EGrowthRate::Fluctuating;
		}
		else
		{
			Rarity = ERarity::Mythical;
			BaseXP = BaseCombatPower * 0.60f;
			LevelGrowthRate = EGrowthRate::Erratic;
		}
	}
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
	DOREPLIFETIME(UCharacterContextComponent, AuraColor);
	DOREPLIFETIME(UCharacterContextComponent, PrimaryType);
	DOREPLIFETIME(UCharacterContextComponent, SecondaryType);
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
