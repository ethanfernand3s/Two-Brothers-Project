// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

#include "TBGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"

void UInventoryWidgetController::BroadcastInitialValues()
{
	/*************					Test | Just for Parasite ATM									      *************/
	
	FTBAttributeInfo HealthInfo = AttributeInfoLibrary::FindAttributeInfo(FTBGameplayTags::Get().Attributes_Health);
	FTBAttributeInfo MaxHealthInfo = AttributeInfoLibrary::FindAttributeInfo(FTBGameplayTags::Get().Attributes_MaxHealth);
	HealthInfo.AttributeValue = ParasiteAttributeSet->GetHealth();
	MaxHealthInfo.AttributeValue = ParasiteAttributeSet->GetMaxHealth();
	CurrentAndMax_AttributeInfoDelegate.Broadcast(HealthInfo, MaxHealthInfo);

	FTBAttributeInfo StrengthInfo = AttributeInfoLibrary::FindAttributeInfo(FTBGameplayTags::Get().Attributes_Strength);
	StrengthInfo.AttributeValue = ParasiteAttributeSet->GetStrength();
	Single_AttributeInfoDelegate.Broadcast(StrengthInfo);

	FTBAttributeInfo CreatureInfo = AttributeInfoLibrary::FindAttributeInfo(FTBGameplayTags::Get().Attributes_Type);
	CreatureInfo.AttributeValue = ParasiteAttributeSet->GetType();
	Type_AttributeInfoDelegate.Broadcast(CreatureInfo);
}

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	
}