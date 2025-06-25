// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

#include "TBGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"
#include "Characters/CharacterContextComponent.h"
#include "Characters/Data/LevelInfo.h"
#include "GameFramework/PlayerState.h"
#include "Player/ParasitePlayerState.h"

void UInventoryWidgetController::BroadcastInitialValues()
{
	// TODO: Add animal AS when currently inhabited
	
	for (const auto& AttributeBinding : ParasiteAttributeSet->TagsToAttributes)
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

	// Broadcast Type
	FTBAttributeInfo CreatureInfo = AttributeInfoLibrary::FindAttributeInfo(FTBGameplayTags::Get().Attributes_Type);
	CreatureInfo.AttributeValue = ParasiteAttributeSet->GetType();
	Type_AttributeInfoDelegate.Broadcast(CreatureInfo);
}

void UInventoryWidgetController::OnXPChanged(int NewXP)
{
	// TODO: Change this to call a GetGrowthRate From either PS or AnimalPawn Automatically with Interface
	const float XPBarPercent = LevelInfoLibrary::GetProgressToNextLevel(NewXP, Cast<AParasitePlayerState>(ParasitePS)->CharacterContextComponent->GetGrowthRate());
	OnXpPercentChangedDelegate.Broadcast(XPBarPercent);
}
void UInventoryWidgetController::OnAttributePointsChanged(int NewAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(NewAttributePoints);
}

void UInventoryWidgetController::OnBiomeChanged(const FBiomeInfo& BiomeInfo)
{
	OnBiomeChangedDelegate.Broadcast(BiomeInfo);
}

void UInventoryWidgetController::OnCharacterNameChanged(const FText& NewCharacterName)
{
	OnCharacterNameChangedDelegate.Broadcast(NewCharacterName);
}

void UInventoryWidgetController::OnLevelChanged(int NewLevel)
{
	OnLevelChangedDelegate.Broadcast(NewLevel);
}

void UInventoryWidgetController::OnTribeDataChanged(const FTribeData& NewTribeData)
{
	OnTribeDataChangedDelegate.Broadcast(NewTribeData);
}

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	// Bind Character Context
	auto* ParasitePlayerState = Cast<AParasitePlayerState>(ParasitePS);
	check(ParasitePlayerState);	
	ParasitePlayerState->CharacterContextComponent->OnXPChanged.AddUObject(this, &UInventoryWidgetController::OnXPChanged);
	ParasitePlayerState->CharacterContextComponent->OnAttributePointsChanged.AddUObject(this, &UInventoryWidgetController::OnAttributePointsChanged);
	ParasitePlayerState->CharacterContextComponent->OnBiomeChanged.AddUObject(this, &UInventoryWidgetController::OnBiomeChanged);
	ParasitePlayerState->CharacterContextComponent->OnCharacterNameChanged.AddUObject(this, &UInventoryWidgetController::OnCharacterNameChanged);
	ParasitePlayerState->CharacterContextComponent->OnLevelChanged.AddUObject(this, &UInventoryWidgetController::OnLevelChanged);
	ParasitePlayerState->CharacterContextComponent->OnTribeDataChanged.AddUObject(this, &UInventoryWidgetController::OnTribeDataChanged);

	// Bind Attributes
	for (const FTagAttributeBinding& Binding : ParasiteAttributeSet->TagsToAttributes)
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

	// Type attribute delegate
	const FGameplayAttribute TypeAttribute = UBaseAttributeSet::GetTypeAttribute();
	ParasiteASC->GetGameplayAttributeValueChangeDelegate(TypeAttribute).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			FTBAttributeInfo TypeInfo = AttributeInfoLibrary::FindAttributeInfo(FTBGameplayTags::Get().Attributes_Type);
			TypeInfo.AttributeValue = Data.NewValue;

			Type_AttributeInfoDelegate.Broadcast(TypeInfo);
		}
	);
}