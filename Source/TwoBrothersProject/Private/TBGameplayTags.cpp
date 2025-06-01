// Fill out your copyright notice in the Description page of Project Settings.


#include "TBGameplayTags.h"

#include "GameplayTagsManager.h"

FTBGameplayTags FTBGameplayTags::GameplayTags;

void FTBGameplayTags::InitializeNativeGameplayTags()
{
	
#pragma region Input
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button")
		);
	GameplayTags.InputTag_Space = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Space"),
		FString("Input Tag for SpaceBar")
		);
	GameplayTags.InputTag_E = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.E"),
		FString("Input Tag for E key")
		);

#pragma endregion Input
}
