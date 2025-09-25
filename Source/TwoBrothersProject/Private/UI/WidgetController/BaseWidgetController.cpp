// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/BaseWidgetController.h"
#include "Player/TBPlayerController.h"
#include "Player/ParasitePlayerState.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"
#include "AbilitySystem/Animal/AnimalAbilitySystemComponent.h"
#include "AbilitySystem/Animal/AnimalAttributeSet.h"
#include "Player/Interfaces/PlayerInterface.h"

FWidgetControllerParams::FWidgetControllerParams(ATBPlayerController* InTBPC, AParasitePlayerState* InPS,
	UParasiteAbilitySystemComponent* InParasiteASC, UParasiteAttributeSet* InParasiteAS, IPlayerInterface* InParasitePI,
	UAnimalAbilitySystemComponent* InAnimalASC, UAnimalAttributeSet* InAnimalAS, IPlayerInterface* InAnimalPI)
{
	
		TBPC       = InTBPC;
		ParasitePS = InPS;
		ParasiteASC = InParasiteASC;
		ParasiteAS  = InParasiteAS;
		ParasitePI  = InParasitePI;
		AnimalASC   = InAnimalASC;
		AnimalAS    = InAnimalAS;
		AnimalPI    = InAnimalPI;
}

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

bool UBaseWidgetController::IsAnimalInhabited() const
{
	return AnimalASC.IsValid() && AnimalPI.IsValid() && AnimalAS.IsValid()
	&& TBPC.IsValid() && TBPC->GetIsAnimalPossessed();
}

UBaseAbilitySystemComponent* UBaseWidgetController::GetActiveASC(bool bIsAnimalPriority) const
{
	if (bIsAnimalPriority)
	{
		return static_cast<UBaseAbilitySystemComponent*>(AnimalASC.Get());
	}
	return static_cast<UBaseAbilitySystemComponent*>(ParasiteASC.Get());
}

// Active AttributeSet
UBaseAttributeSet* UBaseWidgetController::GetActiveAS(bool bIsAnimalPriority) const
{
	if (bIsAnimalPriority)
	{
		return static_cast<UBaseAttributeSet*>(AnimalAS.Get());
	}
	return static_cast<UBaseAttributeSet*>(ParasiteAS.Get());
}

// Active PlayerInterface
IPlayerInterface* UBaseWidgetController::GetActivePI(bool bIsAnimalPriority) const
{
	if (bIsAnimalPriority)
	{
		return AnimalPI.Get();
	}
	return ParasitePI.Get();
}