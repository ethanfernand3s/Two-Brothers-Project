// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"
#include "Characters/CharacterContextComponent.h"
#include "GameFramework/PlayerState.h"
#include "Player/ParasitePlayerState.h"

void UInventoryWidgetController::BroadcastInitialValues()
{
	// TODO: Add animal AS when currently inhabited

#pragma region Combat Attributes
	
	for (const auto& AttributeBinding : ParasiteAttributeSet->TagsToCombatAttributes)
	{
		if (AttributeBinding.HasSecondary())
		{
			// Broadcast Current & Max Vals
			FTBAttributeInfo CurrentInfo = AttributeInfoLibrary::FindAttributeInfo(AttributeBinding.PrimaryTag);
			CurrentInfo.AttributeValue = AttributeBinding.PrimaryAttributeFunc().GetNumericValue(ParasiteAttributeSet);
			
			FTBAttributeInfo MaxInfo = AttributeInfoLibrary::FindAttributeInfo(AttributeBinding.SecondaryTag);
			MaxInfo.AttributeValue = AttributeBinding.SecondaryAttributeFunc().GetNumericValue(ParasiteAttributeSet);

			CurrentAndMax_AttributeInfoDelegate.Broadcast(CurrentInfo, MaxInfo);
		}
		else
		{
			// Broadcast Single Vals
			FTBAttributeInfo Info = AttributeInfoLibrary::FindAttributeInfo(AttributeBinding.PrimaryTag);
			Info.AttributeValue = AttributeBinding.PrimaryAttributeFunc().GetNumericValue(ParasiteAttributeSet);

			Single_AttributeInfoDelegate.Broadcast(Info);
		}
	}

#pragma endregion Combat Attributes

#pragma region Character Context
	
	auto* ParasitePlayerState = Cast<AParasitePlayerState>(ParasitePS);
	check(ParasitePlayerState);
	
	OnAttributePointsChangedDelegate.Broadcast(ParasitePlayerState->CharacterContextComponent->GetAttributePoints());
	OnCharacterNameChangedDelegate.Broadcast(ParasitePlayerState->CharacterContextComponent->GetCharacterName());
	OnLevelChangedDelegate.Broadcast(ParasitePlayerState->CharacterContextComponent->GetLevel());
	OnTribeNameChangedDelegate.Broadcast(ParasitePlayerState->CharacterContextComponent->GetTribeData().TribeName);
	
	OnGenderSetDelegate.Broadcast(ParasitePlayerState->CharacterContextComponent->GetGender());
	OnAuraColorSetDelegate.Broadcast(ParasitePlayerState->CharacterContextComponent->GetAuraColor());
	OnCreatureTypesSetDelegate.Broadcast(ParasitePlayerState->CharacterContextComponent->GetCreatureTypes());
	
#pragma endregion Character Context

	BroadcastAbilityInfo();
}

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	// Bind Character Context
	auto* ParasitePlayerState = Cast<AParasitePlayerState>(ParasitePS);
	check(ParasitePlayerState);	
	
	ParasitePlayerState->CharacterContextComponent->OnAttributePointsChanged.AddUObject(this, &UInventoryWidgetController::OnAttributePointsChanged);
	ParasitePlayerState->CharacterContextComponent->OnCharacterNameChanged.AddUObject(this, &UInventoryWidgetController::OnCharacterNameChanged);
	ParasitePlayerState->CharacterContextComponent->OnLevelChanged.AddUObject(this, &UInventoryWidgetController::OnLevelChanged);
	ParasitePlayerState->CharacterContextComponent->OnTribeDataChanged.AddUObject(this, &UInventoryWidgetController::OnTribeDataChanged);
	
	// Bind Combat Related Attributes
	for (const FTagAttributeBinding& Binding : ParasiteAttributeSet->TagsToCombatAttributes)
	{
		const FGameplayAttribute PrimaryAttribute = Binding.PrimaryAttributeFunc();

		ParasiteASC->GetGameplayAttributeValueChangeDelegate(PrimaryAttribute).AddLambda(
			[this, Binding](const FOnAttributeChangeData& Data)
			{
				FTBAttributeInfo Info = AttributeInfoLibrary::FindAttributeInfo(Binding.PrimaryTag);
				Info.AttributeValue = Data.NewValue;

				if (Binding.HasSecondary())
				{
					FTBAttributeInfo MaxInfo = AttributeInfoLibrary::FindAttributeInfo(Binding.SecondaryTag);
					MaxInfo.AttributeValue = Binding.SecondaryAttributeFunc().GetNumericValue(ParasiteAttributeSet);

					CurrentAndMax_AttributeInfoDelegate.Broadcast(Info, MaxInfo);
				}
				else
				{
					Single_AttributeInfoDelegate.Broadcast(Info);
				}
			}
		);

		if (Binding.HasSecondary())
		{
			const FGameplayAttribute SecondaryAttribute = Binding.SecondaryAttributeFunc();

			ParasiteASC->GetGameplayAttributeValueChangeDelegate(SecondaryAttribute).AddLambda(
				[this, Binding](const FOnAttributeChangeData& Data)
				{
					FTBAttributeInfo MaxInfo = AttributeInfoLibrary::FindAttributeInfo(Binding.SecondaryTag);
					MaxInfo.AttributeValue = Data.NewValue;

					FTBAttributeInfo CurrentInfo = AttributeInfoLibrary::FindAttributeInfo(Binding.PrimaryTag);
					CurrentInfo.AttributeValue = Binding.PrimaryAttributeFunc().GetNumericValue(ParasiteAttributeSet);

					CurrentAndMax_AttributeInfoDelegate.Broadcast(CurrentInfo, MaxInfo);
				}
			);
		}
	}

	// Bind Ability Callbacks

	// TODO: Move to Overlay
	// if (ParasiteASC)
	// {
	// 	// TODO: Equip Abilities
	// 	// ParasiteASC->AbilityEquipped.AddUObject(this, &UInventoryWidgetController::OnAbilityEquipped);
	// 	if (ParasiteASC->bStartupAbilitiesGiven)
	// 	{
	// 		BroadcastAbilityInfo();
	// 	}
	// 	else
	// 	{
	// 		ParasiteASC->AbilitiesGivenDelegate.AddUObject(this, &UInventoryWidgetController::BroadcastAbilityInfo);
	// 	}
	// }
}

void UInventoryWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	ParasiteASC->UpgradeAttribute(AttributeTag);
}

void UInventoryWidgetController::OnTribeDataChanged(const FTribeData& TribeData)
{
	OnTribeNameChangedDelegate.Broadcast(TribeData.TribeName);
}

//TODO: Bring to Overlay don't need this here
/* 
void UInventoryWidgetController::OnXPChanged(int NewXP)
{
	// TODO: Change this to call a GetGrowthRate From either PS or AnimalPawn Automatically with Interface
	const float XPBarPercent = LevelInfoLibrary::GetProgressToNextLevel(NewXP, Cast<AParasitePlayerState>(ParasitePS)->CharacterContextComponent->GetGrowthRate());
	OnXpPercentChangedDelegate.Broadcast(XPBarPercent);
}
*/

void UInventoryWidgetController::OnAttributePointsChanged(int NewAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(NewAttributePoints);
}

void UInventoryWidgetController::OnCharacterNameChanged(const FText& NewCharacterName)
{
	OnCharacterNameChangedDelegate.Broadcast(NewCharacterName);
}

void UInventoryWidgetController::OnLevelChanged(int NewLevel)
{
	OnLevelChangedDelegate.Broadcast(NewLevel);
}