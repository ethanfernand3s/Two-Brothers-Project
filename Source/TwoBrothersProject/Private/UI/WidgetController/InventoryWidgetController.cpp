// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

#include "TBGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"

void UInventoryWidgetController::BroadcastInitialValues()
{
	FTBFloatAttributeInfo HealthInfo = AttributeInfoLibrary::FindFloatAttributeInfo(FTBGameplayTags::Get().Attributes_Health);
	FTBFloatAttributeInfo MaxHealthInfo = AttributeInfoLibrary::FindFloatAttributeInfo(FTBGameplayTags::Get().Attributes_Health);
	
	HealthInfo.AttributeValue = ParasiteAttributeSet->GetHealth();
	MaxHealthInfo.AttributeValue = ParasiteAttributeSet->GetMaxHealth();
	
	Health_AttributeInfoDelegate.Broadcast(HealthInfo, MaxHealthInfo);
}

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	
}