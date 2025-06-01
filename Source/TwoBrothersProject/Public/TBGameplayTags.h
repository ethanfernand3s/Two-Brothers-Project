// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * TB Gameplay Tags
 * 
 * Singleton containing native Gameplay Tags
 */
struct FTBGameplayTags
{
	static const FTBGameplayTags& Get() { return GameplayTags;}
	static void InitializeNativeGameplayTags();

#pragma region Input
	
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_Space;
	FGameplayTag InputTag_E;

#pragma endregion Input

private:
	
	static FTBGameplayTags GameplayTags;
};
