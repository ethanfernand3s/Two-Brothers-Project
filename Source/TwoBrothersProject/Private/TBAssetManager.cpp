// Fill out your copyright notice in the Description page of Project Settings.


#include "TBAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "TBGameplayTags.h"

UTBAssetManager& UTBAssetManager::Get()
{
	check(GEngine);
	
	UTBAssetManager* AuraAssetManager = Cast<UTBAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UTBAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FTBGameplayTags::InitializeNativeGameplayTags();

	// This is required to use Target Data!
	UAbilitySystemGlobals::Get().InitGlobalData();
}
