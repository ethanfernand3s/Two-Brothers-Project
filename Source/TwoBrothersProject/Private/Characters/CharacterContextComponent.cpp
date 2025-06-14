#include "Characters/CharacterContextComponent.h"

#include "Characters/Data/Gender.h"
#include "Net/UnrealNetwork.h"

UCharacterContextComponent::UCharacterContextComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Gender = ECharacterGender::None;
	SetIsReplicatedByDefault(true);
}

void UCharacterContextComponent::InitializeCharacterContext(const FText& InName, int32 InLevel, int32 InXP,
	const FTribeData& InTribeData, ECharacterGender InGender, UBiomeDataAsset* InBiomeData, int32 InAttributePoints)
{
	SetCharacterName(InName);
	SetLevel(InLevel);
	SetXP(InXP);
	SetTribeData(InTribeData);
	SetGender(InGender);
	SetBiomeData(InBiomeData);
	SetAttributePoints(InAttributePoints);

	// Optionally trigger delegates if you want UI to refresh immediately on load
	OnCharacterNameChanged.Broadcast(CharacterName);
	OnLevelChanged.Broadcast(Level);
	OnXPChanged.Broadcast(XP);
	OnTribeDataChanged.Broadcast(TribeData);
	OnBiomeChanged.Broadcast(BiomeData);
	OnAttributePointsChanged.Broadcast(AttributePoints);
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
	OnBiomeChanged.Broadcast(BiomeData);
}

void UCharacterContextComponent::OnRep_AttributePoints()
{
	OnAttributePointsChanged.Broadcast(AttributePoints);
}
