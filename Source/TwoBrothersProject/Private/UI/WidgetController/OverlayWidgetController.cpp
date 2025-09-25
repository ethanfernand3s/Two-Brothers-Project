// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/Animal/AnimalAbilitySystemComponent.h"
#include "Characters/Components/CharacterContextComponent.h"
#include "Characters/Data/LevelInfo.h"
#include "Player/ParasitePlayerState.h"
#include "AbilitySystem/Base/BaseAttributeSet.h"
#include "AbilitySystem/Base/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	// Always broadcast attribute points for both characters
	if (ParasitePI.IsValid() && ParasitePI->GetCharacterContextComponent())
	{
		AttributePointsChanged.Broadcast(ParasitePI->GetCharacterContextComponent()->GetAttributePoints(), true);
		CharacterIconChanged.Broadcast(ParasitePI->GetCharacterContextComponent()->GetCharacterIcon(), true);
	}
	if (AnimalPI.IsValid() && AnimalPI->GetCharacterContextComponent())
	{
		AttributePointsChanged.Broadcast(AnimalPI->GetCharacterContextComponent()->GetAttributePoints(), false);
		CharacterIconChanged.Broadcast(AnimalPI->GetCharacterContextComponent()->GetCharacterIcon(), false);
	}

	// Now only broadcast for the active character
	const bool bIsAnimalActive = IsAnimalInhabited();

	IPlayerInterface* ActivePI = GetActivePI(bIsAnimalActive);
	UBaseAttributeSet* ActiveAS = GetActiveAS(bIsAnimalActive);

	if (ActiveAS)
	{
		HealthChanged.Broadcast(ActiveAS->GetHealth());
		MaxHealthChanged.Broadcast(ActiveAS->GetMaxHealth());
		EnergyChanged.Broadcast(ActiveAS->GetEnergy());
		MaxEnergyChanged.Broadcast(ActiveAS->GetMaxEnergy());
	}

	if (ActivePI && ActivePI->GetCharacterContextComponent())
	{
		const auto* Ctx = ActivePI->GetCharacterContextComponent();
		const float XPBarPercent = LevelInfoLibrary::GetProgressToNextLevel(Ctx->GetXP(), Ctx->GetGrowthRate());
		XPChanged.Broadcast(XPBarPercent);
		LevelChanged.Broadcast(Ctx->GetLevel());
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const bool bIsAnimalActive = IsAnimalInhabited();

	IPlayerInterface* ActivePI = GetActivePI(bIsAnimalActive);
	UBaseAbilitySystemComponent* ActiveASC = GetActiveASC(bIsAnimalActive);
	UBaseAttributeSet* ActiveAS = GetActiveAS(bIsAnimalActive);

	// Only bind the active character's GAS attributes
	if (ActiveASC && ActiveAS)
	{
		BindSelectedAttributes(ActiveASC, ActiveAS);
	}

	// Always bind attribute points for BOTH
	if (ParasitePI.IsValid() && ParasitePI->GetCharacterContextComponent())
	{
		ParasitePI->GetCharacterContextComponent()->OnAttributePointsChanged.AddLambda(
			[this](int32 NewPoints)
			{
				AttributePointsChanged.Broadcast(NewPoints, true);
			});
		ParasitePI->GetCharacterContextComponent()->OnCharacterIconChanged.AddLambda(
			[this](UTexture2D* NewCharacterIcon)
			{
				CharacterIconChanged.Broadcast(NewCharacterIcon, true);
			});
	}
	if (AnimalPI.IsValid() && AnimalPI->GetCharacterContextComponent())
	{
		AnimalPI->GetCharacterContextComponent()->OnAttributePointsChanged.AddLambda(
			[this](int32 NewPoints)
			{
				AttributePointsChanged.Broadcast(NewPoints, false);
			});
		AnimalPI->GetCharacterContextComponent()->OnCharacterIconChanged.AddLambda(
			[this](UTexture2D* NewCharacterIcon)
			{
				CharacterIconChanged.Broadcast(NewCharacterIcon, true);
			});
	}

	// Bind level + XP only for the active character
	if (ActivePI && ActivePI->GetCharacterContextComponent())
	{
		auto* Ctx = ActivePI->GetCharacterContextComponent();
		Ctx->OnXPChanged.AddLambda([this, Ctx](int32 NewXP)
		{
			const float XPBarPercent = LevelInfoLibrary::GetProgressToNextLevel(NewXP, Ctx->GetGrowthRate());
			XPChanged.Broadcast(XPBarPercent);
		});

		Ctx->OnLevelChanged.AddLambda([this](int32 NewLevel)
		{
			LevelChanged.Broadcast(NewLevel);
		});
	}
}

void UOverlayWidgetController::BindSelectedAttributes(
	UAbilitySystemComponent* ASC, UBaseAttributeSet* AS)
{
	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			HealthChanged.Broadcast(Data.NewValue);
		});

	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			MaxHealthChanged.Broadcast(Data.NewValue);
		});

	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetEnergyAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			EnergyChanged.Broadcast(Data.NewValue);
		});

	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxEnergyAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			MaxEnergyChanged.Broadcast(Data.NewValue);
		});
}