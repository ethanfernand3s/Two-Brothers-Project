// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/BaseWidgetController.h"

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
