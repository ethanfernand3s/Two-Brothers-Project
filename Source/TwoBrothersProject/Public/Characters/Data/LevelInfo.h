// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class TWOBROTHERSPROJECT_API LevelInfoLibrary
{
public:
	
	static int32 GetLevelFromXP(float CurrentXP);
	static float GetProgressToNextLevel(float CurrentXP);
};
