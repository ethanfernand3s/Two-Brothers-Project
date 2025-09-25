// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"
#include "Characters/Components/CharacterContextComponent.h"
#include "Characters/Data/LevelInfo.h"
#include "Player/ParasitePlayerState.h"
#include "UI/WidgetController/InventoryWidgetController.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	auto* ParasitePlayerState = Cast<AParasitePlayerState>(ParasitePS);
	check(ParasitePlayerState);
	
#pragma region GAS

	HealthChanged.Broadcast(ParasiteAttributeSet->GetHealth());
	MaxHealthChanged.Broadcast(ParasiteAttributeSet->GetMaxHealth());
	EnergyChanged.Broadcast(ParasiteAttributeSet->GetEnergy());
	MaxEnergyChanged.Broadcast(ParasiteAttributeSet->GetMaxEnergy());

#pragma endregion GAS

#pragma region Character Context

	AttributePointsChanged.Broadcast(ParasitePlayerState->CharacterContextComponent->GetAttributePoints());

	const float XPBarPercent = LevelInfoLibrary::GetProgressToNextLevel(ParasitePlayerState->CharacterContextComponent->GetXP(),
		Cast<AParasitePlayerState>(ParasitePS)->CharacterContextComponent->GetGrowthRate());
	XPChanged.Broadcast(XPBarPercent);

	LevelChanged.Broadcast(ParasitePlayerState->CharacterContextComponent->GetLevel());

#pragma endregion Character Context
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	auto* ParasitePlayerState = Cast<AParasitePlayerState>(ParasitePS);
	check(ParasitePlayerState);

	// TODO: REFACTOR !!!
	
	// Right here we would probably just display the primary whether that's the parasite with no inhabitant or
		// if an animal is inhabited

#pragma region GAS
	
	ParasiteASC->GetGameplayAttributeValueChangeDelegate(ParasiteAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				HealthChanged.Broadcast(Data.NewValue);
			}
		);

	ParasiteASC->GetGameplayAttributeValueChangeDelegate(ParasiteAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				MaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	ParasiteASC->GetGameplayAttributeValueChangeDelegate(ParasiteAttributeSet->GetEnergyAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				EnergyChanged.Broadcast(Data.NewValue);
			}
		);

	ParasiteASC->GetGameplayAttributeValueChangeDelegate(ParasiteAttributeSet->GetMaxEnergyAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				MaxEnergyChanged.Broadcast(Data.NewValue);
			}
		);

#pragma endregion GAS

#pragma region Character Context
	
	ParasitePlayerState->CharacterContextComponent->OnAttributePointsChanged.AddLambda(
		[this](int32 NewAttributePoints)
		{
			AttributePointsChanged.Broadcast(NewAttributePoints);
		});
	ParasitePlayerState->CharacterContextComponent->OnXPChanged.AddLambda(
		[this](int32 NewXP)
		{
			// TODO: Change this to call a GetGrowthRate From either PS or AnimalPawn Automatically with Interface
			const float XPBarPercent = LevelInfoLibrary::GetProgressToNextLevel(NewXP, Cast<AParasitePlayerState>(ParasitePS)->CharacterContextComponent->GetGrowthRate());
			XPChanged.Broadcast(XPBarPercent);
		});
	ParasitePlayerState->CharacterContextComponent->OnLevelChanged.AddLambda(
		[this](int32 NewLevel)
		{
			LevelChanged.Broadcast(NewLevel);
		});

#pragma endregion Character Context
	
	// Bind Ability Callbacks
	
	if (ParasiteASC)
	{
		// TODO: Equip Abilities
		//ParasiteASC->AbilityEquipped.AddUObject(this, &UInventoryWidgetController::OnAbilityEquipped);
		if (ParasiteASC->bStartupAbilitiesGiven)
		{
			// BroadcastAbilityInfo();
		}
		else
		{
			// ParasiteASC->AbilitiesGivenDelegate.AddUObject(this, &Super::BroadcastAbilityInfo);
		}
	}
}