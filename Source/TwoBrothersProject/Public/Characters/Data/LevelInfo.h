// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

enum class EGrowthRate : uint8;

class TWOBROTHERSPROJECT_API LevelInfoLibrary
{
public:
	
	static int32 GetLevelFromXP(float CurrentXP, EGrowthRate Rate);
	static float GetProgressToNextLevel(float CurrentXP, EGrowthRate Rate);
	static int32 GetXPReward(int32 VictimBaseXP, int32 VictimLevel, bool bIsCurrentlyInhabited, float AdditionalModiferMultiplier = 1);
};
