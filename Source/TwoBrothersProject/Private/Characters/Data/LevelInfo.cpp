// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Data/LevelInfo.h"
#include "PrecomputedValues/PrecomputedXPTable.h"
#include "Characters/Data/GrowthRate.h"

static constexpr const float* GetXPTable(EGrowthRate Rate)
{
	switch (Rate)
	{
	case EGrowthRate::Fast:          return PrecomputedXP_Fast;
	case EGrowthRate::MediumFast:    return PrecomputedXP_MediumFast;
	case EGrowthRate::MediumSlow:    return PrecomputedXP_MediumSlow;
	case EGrowthRate::Slow:          return PrecomputedXP_Slow;
	case EGrowthRate::Erratic:       return PrecomputedXP_Erratic;
	case EGrowthRate::Fluctuating:   return PrecomputedXP_Fluctuating;
	default:                         return PrecomputedXP_MediumFast; // safe fallback
	}
}
// Binary search to determine current level from XP and growth rate
int32 LevelInfoLibrary::GetLevelFromXP(float CurrentXP, EGrowthRate Rate)
{
	const float* Table = GetXPTable(Rate);

	int32 Low = 0;
	int32 High = XPLevelCount - 1;
	int32 Level = 0;

	while (Low <= High)
	{
		int32 Mid = (Low + High) / 2;
		if (Table[Mid] <= CurrentXP)
		{
			Level = Mid;
			Low   = Mid + 1;
		}
		else
		{
			High = Mid - 1;
		}
	}
	return Level + 1;             // human-friendly level (starts at 1)
}

// 0-1 progress bar showing how close we are to next level
float LevelInfoLibrary::GetProgressToNextLevel(float CurrentXP, EGrowthRate Rate)
{
	const float* Table = GetXPTable(Rate);

	const int32 Level      = GetLevelFromXP(CurrentXP, Rate);
	const float XPCurrent  = Table[Level - 1];
	const float XPNext     = (Level < XPLevelCount) ? Table[Level] : XPCurrent;

	return (XPNext > XPCurrent)
		? (CurrentXP - XPCurrent) / (XPNext - XPCurrent)
		: 1.f;
}

int32 LevelInfoLibrary::GetXPReward(int32 VictimBaseXP, int32 VictimLevel, bool bIsCurrentlyInhabited,
	float AdditionalModiferMultiplier)
{
	float InhabitedMultiplier{1.f};
	if (bIsCurrentlyInhabited) InhabitedMultiplier = 1.5f;

	return (VictimBaseXP * VictimLevel * InhabitedMultiplier * AdditionalModiferMultiplier);
}

