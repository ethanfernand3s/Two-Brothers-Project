// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/BaseWidgetController.h"
#include "Game/TBGamemode.h"
#include "Inventory/Components/TBInventoryComponent.h"

void UBaseWidgetController::SetWidgetControllerParams(const TUniquePtr<FWidgetControllerParams>& WCParams)
{
	TBPC = WCParams->TBPC;
	ParasitePS = WCParams->ParasitePS;
	ParasiteASC = WCParams->ParasiteASC;
	ParasiteAS = WCParams->ParasiteAS;
	AnimalASC = WCParams->AnimalASC;
	AnimalAS = WCParams->AnimalAS;
	ParasitePI = WCParams->ParasitePI;
	AnimalPI = WCParams->AnimalPI;
}

void UBaseWidgetController::BroadcastInitialValues()
{
}

void UBaseWidgetController::BindCallbacksToDependencies()
{
}

const UUIDataAsset* UBaseWidgetController::GetUIDataAsset() const
{
	return UIDataAsset;
}
