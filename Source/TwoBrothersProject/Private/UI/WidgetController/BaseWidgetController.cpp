// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/BaseWidgetController.h"

#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"

void UBaseWidgetController::SetWidgetControllerParams(const TUniquePtr<FWidgetControllerParams>& WCParams)
{
	TBPlayerController = WCParams->TBPlayerController;
	ParasitePS = WCParams->ParasitePS;
	ParasiteASC = WCParams->ParasiteAsc;
	ParasiteAttributeSet = WCParams->ParasiteAttributeSet;
	AnimalASC = WCParams->AnimalAsc;
	AnimalAttributeSet = WCParams->AnimalAttributeSet;
}

void UBaseWidgetController::BroadcastInitialValues()
{
}

void UBaseWidgetController::BindCallbacksToDependencies()
{
}

void UBaseWidgetController::BroadcastAbilityInfo()
{
	if (ParasiteASC->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FTBAbilityInfo* Info = AbilityInfo->AbilityMap.Find(ParasiteASC->GetAbilityTagFromSpec(AbilitySpec));
		if (Info)
		{
			Info->InputTag = ParasiteASC->GetInputTagFromSpec(AbilitySpec);
			Info->StatusTag = ParasiteASC->GetStatusFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(*Info);
		}
		
	});
	ParasiteASC->ForEachAbility(BroadcastDelegate);
}
