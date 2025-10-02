// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/BaseWidgetController.h"
#include "Player/TBPlayerController.h"
#include "Player/ParasitePlayerState.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"
#include "AbilitySystem/Animal/AnimalAbilitySystemComponent.h"
#include "AbilitySystem/Animal/AnimalAttributeSet.h"
#include "Characters/Components/CharacterContextComponent.h"
#include "Characters/Data/LevelInfo.h"
#include "Characters/Data/TribeData.h"
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

void UBaseWidgetController::RebindToDependencies()
{
	if (!IsAnimalInhabited()) UnBindCallbacks();
	
	BindCallbacksToDependencies();
	BroadcastInitialValues();
	OnWidgetControllerRebound.Broadcast(IsAnimalInhabited());
}

void UBaseWidgetController::BroadcastInitialValues()
{
	if (ParasitePI.IsValid() && ParasitePI->GetCharacterContextComponent())
	{
		BroadcastCharacterContext(ParasitePI->GetCharacterContextComponent(), true);
	}
	if (AnimalPI.IsValid() && AnimalPI->GetCharacterContextComponent())
	{
		BroadcastCharacterContext(AnimalPI->GetCharacterContextComponent(), false);
	}
}

void UBaseWidgetController::BroadcastCharacterContext(UCharacterContextComponent* CharacterContextComponent, bool bIsParasiteVal) const
{
	OnAttributePointsChangedDelegate.Broadcast(CharacterContextComponent->GetAttributePoints(), bIsParasiteVal);
	OnLevelChangedDelegate.Broadcast(CharacterContextComponent->GetLevel(), bIsParasiteVal);
	OnXPChangedDelegate.Broadcast(GetXPBarPercent(CharacterContextComponent->GetXP(), CharacterContextComponent), bIsParasiteVal);
	OnCharacterNameChangedDelegate.Broadcast(CharacterContextComponent->GetCharacterName(), bIsParasiteVal);
	OnCharacterIconChanged.Broadcast(CharacterContextComponent->GetCharacterIcon(), bIsParasiteVal);
	OnTribeDataChangedDelegate.Broadcast(CharacterContextComponent->GetTribeData(), bIsParasiteVal);
	
	OnGenderSetDelegate.Broadcast(CharacterContextComponent->GetGender(), bIsParasiteVal);
	OnCreatureTypesSetDelegate.Broadcast(CharacterContextComponent->GetCreatureTypes(), bIsParasiteVal);
	OnRaritySetDelegate.Broadcast(CharacterContextComponent->GetRarity(), bIsParasiteVal);
}


void UBaseWidgetController::BindCallbacksToDependencies()
{
	if (ParasitePI.IsValid() && ParasitePI->GetCharacterContextComponent())
	{
		BindAllCharacterContext(ParasitePI->GetCharacterContextComponent(), true);
	}
	if (AnimalPI.IsValid() && AnimalPI->GetCharacterContextComponent())
	{
		BindAllCharacterContext(AnimalPI->GetCharacterContextComponent(), false);
	}
}

void UBaseWidgetController::BindAllCharacterContext(UCharacterContextComponent* CharacterContextComponent, bool bIsParasiteVal)
{
	FCharacterContextHandles ContextHandles;
	ContextHandles.AttributePointsChangedHandle = CharacterContextComponent->OnAttributePointsChanged.AddLambda(
		[this, bIsParasiteVal](int NewAttributePoints)
		{
			OnAttributePointsChangedDelegate.Broadcast(NewAttributePoints, bIsParasiteVal);
		});
	
	ContextHandles.LevelChangedHandle = CharacterContextComponent->OnLevelChanged.AddLambda(
			[this, bIsParasiteVal](int NewLevel)
			{
				OnLevelChangedDelegate.Broadcast(NewLevel, bIsParasiteVal);
			});

	ContextHandles.XPChangedHandle = CharacterContextComponent->OnXPChanged.AddLambda(
			[this, bIsParasiteVal, CharacterContextComponent](int32 NewXP)
			{
				OnXPChangedDelegate.Broadcast(GetXPBarPercent(NewXP, CharacterContextComponent), bIsParasiteVal);
			});
	
	ContextHandles.CharacterNameChangedHandle = CharacterContextComponent->OnCharacterNameChanged.AddLambda(
		[this, bIsParasiteVal](const FText& NewCharacterName)
		{
			OnCharacterNameChangedDelegate.Broadcast(NewCharacterName, bIsParasiteVal);
		});

	ContextHandles.CharacterIconChangedHandle = CharacterContextComponent->OnCharacterIconChanged.AddLambda(
		[this, bIsParasiteVal](UTexture2D* NewCharacterIcon)
		{
			OnCharacterIconChanged.Broadcast(NewCharacterIcon, bIsParasiteVal);
		});
	
	ContextHandles.TribeDataChangedHandle = CharacterContextComponent->OnTribeDataChanged.AddLambda(
		[this, bIsParasiteVal](const FTribeData& TribeData)
		{
			OnTribeDataChangedDelegate.Broadcast(TribeData, bIsParasiteVal);
		});
	
	if (!bIsParasiteVal) AnimalContextHandles = ContextHandles;
}

void UBaseWidgetController::UnBindCallbacks()
{
	UnbindAnimalCharacterContext();
}

void UBaseWidgetController::UnbindAnimalCharacterContext()
{
	if (!AnimalContextHandles.IsSet()) return;
	
	IPlayerInterface* PlayerInterface = AnimalPI.Get();
	if (!PlayerInterface) return;

	UCharacterContextComponent* Context = PlayerInterface->GetCharacterContextComponent();
	if (!IsValid(Context)) return;

	Context->OnAttributePointsChanged.Remove(AnimalContextHandles->AttributePointsChangedHandle);
	Context->OnLevelChanged.Remove(AnimalContextHandles->LevelChangedHandle);
	Context->OnCharacterNameChanged.Remove(AnimalContextHandles->CharacterNameChangedHandle);
	Context->OnCharacterIconChanged.Remove(AnimalContextHandles->CharacterIconChangedHandle);
	Context->OnTribeDataChanged.Remove(AnimalContextHandles->TribeDataChangedHandle);

	AnimalContextHandles.Reset();
}

const UUIDataAsset* UBaseWidgetController::GetUIDataAsset() const
{
	return UIDataAsset;
}

bool UBaseWidgetController::IsAnimalInhabited() const
{
	return AnimalASC.IsValid() && AnimalPI.IsValid() && AnimalAS.IsValid();
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

float UBaseWidgetController::GetXPBarPercent(int32 XPAmount, const UCharacterContextComponent* Context)
{
	return (IsValid(Context)) ? LevelInfoLibrary::GetProgressToNextLevel(XPAmount, Context->GetGrowthRate()) : 0.f;
}
