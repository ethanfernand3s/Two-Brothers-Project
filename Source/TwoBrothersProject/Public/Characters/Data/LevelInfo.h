// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class TWOBROTHERSPROJECT_API LevelInfoLibrary
{
public:
	
	static int32 GetLevelFromXP(float CurrentXP);
	static float GetProgressToNextLevel(float CurrentXP);
	static int32 GetXPReward(int32 VictimBaseXP, int32 VictimLevel, bool bIsCurrentlyInhabited, float AdditionalModiferMultiplier = 1);
};