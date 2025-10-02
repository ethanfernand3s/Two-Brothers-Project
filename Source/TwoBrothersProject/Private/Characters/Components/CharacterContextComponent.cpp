#include "Characters/Components/CharacterContextComponent.h"

#include "TBGameplayTags.h"
#include "Characters/Data/GrowthRate.h"
#include "Characters/Data/LevelInfo.h"
#include "Net/UnrealNetwork.h"

UCharacterContextComponent::UCharacterContextComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Gender = FGameplayTag::EmptyTag;
	Rarity = FGameplayTag::EmptyTag;
	CreatureTypeTags = FGameplayTagContainer::EmptyContainer;
	BaseXP = 0.f;
	BaseCombatPower = -1.f;
	AttributePoints = 0;
	Level = 1;
	SetIsReplicatedByDefault(true);
}

void UCharacterContextComponent::InitializeComponent()
{
	Super::InitializeComponent();
	InitializeCharacterContext();
}

void UCharacterContextComponent::InitializeCharacterContext(float ExternalBaseCombatPower)
{
	SetLevel(LevelInfoLibrary::GetLevelFromXP(XP, LevelGrowthRate));
	SetAttributePoints(Level);
	InitializeCombatRelatedVars(ExternalBaseCombatPower);
}

void UCharacterContextComponent::InitializeCombatRelatedVars(float ExternalBaseCombatPower)
{
	// If combat power isn't set manually then set randomly
	if (BaseCombatPower == -1.f && ExternalBaseCombatPower == -1.f)
	{
		InitializeCombatPowerRandomly();
	}
	else if (ExternalBaseCombatPower != -1.f)
	{
		BaseCombatPower = ExternalBaseCombatPower;
	}
	
	// Add IV's to BaseCombatPower
	InitializeRandomIVs();
	BaseCombatPower += IVSet.GetTotal();

	const auto& Tags = FTBGameplayTags::Get();
	
	if (BaseCombatPower < 300.f)
	{
		Rarity = Tags.Rarities_Common;
		BaseXP = BaseCombatPower * 0.18f;
		LevelGrowthRate = EGrowthRate::Fast;
	}
	else if (BaseCombatPower < 400.f)
	{
		Rarity = Tags.Rarities_Uncommon;
		BaseXP = BaseCombatPower * 0.22f;
		LevelGrowthRate = EGrowthRate::MediumFast;
	}
	else if (BaseCombatPower < 500.f)
	{
		Rarity = Tags.Rarities_Rare;
		BaseXP = BaseCombatPower * 0.31f;
		LevelGrowthRate = EGrowthRate::MediumSlow;
	}
	else if (BaseCombatPower < 650.f)
	{
		Rarity = Tags.Rarities_Epic;
		BaseXP = BaseCombatPower * 0.40f;
		LevelGrowthRate = EGrowthRate::Slow;
	}
	else if (BaseCombatPower < 800.f)
	{
		Rarity = Tags.Rarities_Legendary;
		BaseXP = BaseCombatPower * 0.50f;
		LevelGrowthRate = EGrowthRate::Fluctuating;
	}
	else
	{
		Rarity = Tags.Rarities_Prismatic;
		BaseXP = BaseCombatPower * 0.60f;
		LevelGrowthRate = EGrowthRate::Erratic;
	}
}

void UCharacterContextComponent::InitializeCombatPowerRandomly()
{
	// === Randomly roll rarity and base CP ===
	float Roll = FMath::FRand();
	
	// Leave the rest to chance
	if (Roll < 0.4f)
	{
		BaseCombatPower += FMath::RandRange(200, 300);
	}
	else if (Roll < 0.65f)
	{
		BaseCombatPower += FMath::RandRange(300, 400);
	}
	else if (Roll < 0.80f)
	{
		BaseCombatPower += FMath::RandRange(400, 500);
	}
	else if (Roll < 0.95f)
	{
		BaseCombatPower += FMath::RandRange(500, 600);
	}
	else if (Roll < 0.99f)
	{
		BaseCombatPower += FMath::RandRange(650, 750);
	}
	else
	{
		BaseCombatPower += FMath::RandRange(800, 1000);
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

	// TODO: Convert to push model and limit using replicated, prefer On_Rep
	DOREPLIFETIME(UCharacterContextComponent, CharacterName);
	DOREPLIFETIME(UCharacterContextComponent, Level);
	DOREPLIFETIME(UCharacterContextComponent, XP);
	DOREPLIFETIME(UCharacterContextComponent, TribeData);
	DOREPLIFETIME(UCharacterContextComponent, Gender);
	DOREPLIFETIME(UCharacterContextComponent, AttributePoints);
	DOREPLIFETIME(UCharacterContextComponent, Rarity);
	DOREPLIFETIME(UCharacterContextComponent, BaseXP);
	DOREPLIFETIME(UCharacterContextComponent, IVSet);
	DOREPLIFETIME(UCharacterContextComponent, LevelGrowthRate);
	DOREPLIFETIME(UCharacterContextComponent, CreatureTypeTags);
	DOREPLIFETIME(UCharacterContextComponent, BaseCombatPower);
	DOREPLIFETIME(UCharacterContextComponent, BodyPartTags);
	DOREPLIFETIME(UCharacterContextComponent, CharacterIcon);
}

void UCharacterContextComponent::OnRep_CharacterName()
{
	OnCharacterNameChanged.Broadcast(CharacterName);
}

void UCharacterContextComponent::OnRep_CharacterIcon()
{
	OnCharacterIconChanged.Broadcast(CharacterIcon);
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

void UCharacterContextComponent::OnRep_AttributePoints()
{
	OnAttributePointsChanged.Broadcast(AttributePoints);
}