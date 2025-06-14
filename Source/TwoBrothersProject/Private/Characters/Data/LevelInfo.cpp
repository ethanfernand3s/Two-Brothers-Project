// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Data/LevelInfo.h"
#include "PrecomputedValues/PrecomputedXP.h"

// Binary search to determine current level from XP
int32 LevelInfoLibrary::GetLevelFromXP(float CurrentXP)
{
	int32 Low = 0, High = XPLevelCount - 1;
	int32 Level = 0;

	while (Low <= High)
	{
		int32 Mid = (Low + High) / 2;
		if (PrecomputedXP[Mid] <= CurrentXP)
		{
			Level = Mid;
			Low = Mid + 1;
		}
		else
		{
			High = Mid - 1;
		}
	}

	return Level + 1;
}

float LevelInfoLibrary::GetProgressToNextLevel(float CurrentXP)
{
	int32 Level = GetLevelFromXP(CurrentXP);
	float XPCurrent = PrecomputedXP[Level - 1];
	float XPNext = (Level < XPLevelCount) ? PrecomputedXP[Level] : XPCurrent;

	return (XPNext - XPCurrent > 0.f)
		? (CurrentXP - XPCurrent) / (XPNext - XPCurrent)
		: 1.f;
}

