// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Data/UIDataAsset.h"

const FLinearColor* UUIDataAsset::GetRarityColor(const FGameplayTag& Key) const
{
	return RarityColorMap.Find(Key);
}

UTexture2D* UUIDataAsset::GetGenderTexture(const FGameplayTag& Key) const
{
	UTexture2D* const* GenderTexturePtr = GenderTexturesMap.Find(Key);
	if (!GenderTexturePtr) return nullptr;
	
	return *GenderTexturePtr;
}

const FCreatureTypeImages* UUIDataAsset::GetCreatureTypeImages(const FGameplayTag& Key) const
{
	return CreatureTypeImagesMap.Find(Key);
}
