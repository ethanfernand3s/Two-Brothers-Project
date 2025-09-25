// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetController/InventoryWidgetController.h"

#include "AbilitySystem/Animal/AnimalAbilitySystemComponent.h"
#include "AbilitySystem/Animal/AnimalAttributeSet.h"
#include "AbilitySystem/Base/BaseAttributeSet.h"
#include "AbilitySystem/Base/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"
#include "Characters/Components/CharacterContextComponent.h"
#include "Inventory/Components/TBInventoryComponent.h"
#include "Player/ParasitePlayerState.h"
#include "UI/HUD/PlayerHUD.h"

void UInventoryWidgetController::BroadcastInitialValues()
{
	// Parasite
	if (ParasiteAS.IsValid())
	{
		BroadcastAllAttributes(ParasiteAS.Get(), true);
	}
	if (ParasitePI.IsValid() && ParasitePI->GetCharacterContextComponent())
	{
		BroadcastCharacterContext(ParasitePI->GetCharacterContextComponent(), true);
	}
	
	// Animal
	if (AnimalAS.IsValid())
	{
		BroadcastAllAttributes(AnimalAS.Get(),false);
	}
	if (AnimalPI.IsValid() && AnimalPI->GetCharacterContextComponent())
	{
		BroadcastCharacterContext(AnimalPI->GetCharacterContextComponent(), false);
	}

	// Inventory
	if (ParasitePS.IsValid())
	{
		if (UTBInventoryComponent* Inv = ParasitePS->FindComponentByClass<UTBInventoryComponent>())
		{
			CachedInventory = Inv;
		}
	}
}

// Loop over all bindings in an AttributeSet and broadcast values
void UInventoryWidgetController::BroadcastAllAttributes(UBaseAttributeSet* AttributeSet, bool bIsParasiteVal) const
{
	for (const FTagAttributeBinding& Binding : AttributeSet->TagsToCombatAttributes)
	{
		if (Binding.HasSecondary())
		{
			BroadcastAttributePair(Binding, AttributeSet, bIsParasiteVal);
		}
		else
		{
			BroadcastSingleAttribute(Binding, AttributeSet, bIsParasiteVal);
		}
	}
}

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	// Bind Character Context
	
	// Parasite
	if (ParasiteASC.IsValid() && ParasiteAS.IsValid())
	{
		BindAllAttributeCallbacks(ParasiteASC.Get(), ParasiteAS.Get(), true);
	}
	if (ParasitePI.IsValid() && ParasitePI->GetCharacterContextComponent())
	{
		BindAllCharacterContext(ParasitePI->GetCharacterContextComponent(), true);
	}
	
	// Animal
	if (AnimalASC.IsValid() && AnimalAS.IsValid())
	{
		BindAllAttributeCallbacks(AnimalASC.Get(), AnimalAS.Get(), false);
	}
	if (AnimalPI.IsValid() && AnimalPI->GetCharacterContextComponent())
	{
		BindAllCharacterContext(AnimalPI->GetCharacterContextComponent(), false);
	}
}

// Loop over all bindings in an AttributeSet and bind callbacks
void UInventoryWidgetController::BindAllAttributeCallbacks(UAbilitySystemComponent* ASC, UBaseAttributeSet* AttributeSet,
														   bool bIsParasiteVal) const
{
	for (const FTagAttributeBinding& Binding : AttributeSet->TagsToCombatAttributes)
	{
		BindAttributeCallback(ASC, Binding, AttributeSet, bIsParasiteVal);
	}
}

// Bind attribute change callbacks for one binding
void UInventoryWidgetController::BindAttributeCallback( UAbilitySystemComponent* ASC,
														const FTagAttributeBinding& Binding,
														UBaseAttributeSet* AttributeSet,
														bool bIsParasiteVal) const
{
	const FGameplayAttribute PrimaryAttribute = Binding.PrimaryAttributeFunc();

	// Bind Primary
	ASC->GetGameplayAttributeValueChangeDelegate(PrimaryAttribute).AddLambda(
		[this, Binding, AttributeSet, bIsParasiteVal](const FOnAttributeChangeData& Data)
		{
			if (Binding.HasSecondary())
			{
				BroadcastAttributePair(Binding, AttributeSet, bIsParasiteVal);
			}
			else
			{
				BroadcastSingleAttribute(Binding, AttributeSet, bIsParasiteVal);
			}
		}
	);

	// Bind Secondary (if it exists)
	if (Binding.HasSecondary())
	{
		const FGameplayAttribute SecondaryAttribute = Binding.SecondaryAttributeFunc();

		ASC->GetGameplayAttributeValueChangeDelegate(SecondaryAttribute).AddLambda(
			[this, Binding, AttributeSet, bIsParasiteVal](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributePair(Binding, AttributeSet, bIsParasiteVal);
			}
		);
	}
}

// Broadcast a Primary/Secondary attribute pair (Current + Max)
void UInventoryWidgetController::BroadcastAttributePair(const FTagAttributeBinding& Binding, 
													    UBaseAttributeSet* AttributeSet,
													    bool bIsParasiteVal) const
{
	FTBAttributeInfo CurrentInfo = AttributeInfoLibrary::FindAttributeInfo(Binding.PrimaryTag);
	CurrentInfo.AttributeValue = Binding.PrimaryAttributeFunc().GetNumericValue(AttributeSet);

	FTBAttributeInfo MaxInfo = AttributeInfoLibrary::FindAttributeInfo(Binding.SecondaryTag);
	MaxInfo.AttributeValue = Binding.SecondaryAttributeFunc().GetNumericValue(AttributeSet);

	CurrentAndMax_AttributeInfoDelegate.Broadcast(CurrentInfo, MaxInfo, bIsParasiteVal);
}

// Broadcast a single attribute (no secondary)
void UInventoryWidgetController::BroadcastSingleAttribute(const FTagAttributeBinding& Binding, 
														  UBaseAttributeSet* AttributeSet,
														  bool bIsParasiteVal) const
{
	FTBAttributeInfo Info = AttributeInfoLibrary::FindAttributeInfo(Binding.PrimaryTag);
	Info.AttributeValue = Binding.PrimaryAttributeFunc().GetNumericValue(AttributeSet);

	Single_AttributeInfoDelegate.Broadcast(Info, bIsParasiteVal);
}

void UInventoryWidgetController::BroadcastCharacterContext(UCharacterContextComponent* CharacterContextComponent, bool bIsParasiteVal) const
{
	OnAttributePointsChangedDelegate.Broadcast(CharacterContextComponent->GetAttributePoints(), bIsParasiteVal);
	OnCharacterNameChangedDelegate.Broadcast(CharacterContextComponent->GetCharacterName(), bIsParasiteVal);
	OnLevelChangedDelegate.Broadcast(CharacterContextComponent->GetLevel(), bIsParasiteVal);
	OnTribeNameChangedDelegate.Broadcast(CharacterContextComponent->GetTribeData().TribeName, bIsParasiteVal);
	OnRaritySetDelegate.Broadcast(CharacterContextComponent->GetRarity(), bIsParasiteVal);

	OnGenderSetDelegate.Broadcast(CharacterContextComponent->GetGender(), bIsParasiteVal);
	OnCreatureTypesSetDelegate.Broadcast(CharacterContextComponent->GetCreatureTypes(), bIsParasiteVal);
}

void UInventoryWidgetController::BindAllCharacterContext(UCharacterContextComponent* CharacterContextComponent, bool bIsParasiteVal) const
{
	CharacterContextComponent->OnAttributePointsChanged.AddLambda(
		[this, bIsParasiteVal](int NewAttributePoints)
		{
			OnAttributePointsChangedDelegate.Broadcast(NewAttributePoints, bIsParasiteVal);
		});

	CharacterContextComponent->OnCharacterNameChanged.AddLambda(
		[this, bIsParasiteVal](const FText& NewCharacterName)
		{
			OnCharacterNameChangedDelegate.Broadcast(NewCharacterName, bIsParasiteVal);
		});

	CharacterContextComponent->OnCharacterIconChanged.AddLambda(
		[this, bIsParasiteVal](UTexture2D* NewCharacterIcon)
		{
			CharacterIconChanged.Broadcast(NewCharacterIcon, bIsParasiteVal);
		});

	CharacterContextComponent->OnLevelChanged.AddLambda(
		[this, bIsParasiteVal](int NewLevel)
		{
			OnLevelChangedDelegate.Broadcast(NewLevel, bIsParasiteVal);
		});

	CharacterContextComponent->OnTribeDataChanged.AddLambda(
		[this, bIsParasiteVal](const FTribeData& TribeData)
		{
			OnTribeNameChangedDelegate.Broadcast(TribeData.TribeName, bIsParasiteVal);
		});
}

void UInventoryWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag) const
{
	UBaseAbilitySystemComponent* ActiveASC = GetActiveASC(!bIsParasiteFocusedCharacter);
	
	if (IsValid(ActiveASC)) ActiveASC->UpgradeAttribute(AttributeTag);
}

void UInventoryWidgetController::TryUnlockItem(UTBInventoryItem* Item, bool bIsEquippableSlot) const
{
	IPlayerInterface* ActivePI = GetActivePI(!bIsParasiteFocusedCharacter);
	if (!ActivePI) return;
	
	const UCharacterContextComponent* CharacterContextComp = ActivePI->GetCharacterContextComponent();
	if (!IsValid(CharacterContextComp)) return;
	
	CachedInventory->Server_TryUnlockItem(Item, bIsEquippableSlot, CharacterContextComp);
}

void UInventoryWidgetController::HandleAbilityStatusChanged(const UTBInventoryItem* Item, FGameplayTag SlotInputTag)
{
	UBaseAbilitySystemComponent* ActiveASC = GetActiveASC(!bIsParasiteFocusedCharacter);
	if (IsValid(ActiveASC)) ActiveASC->HandleAbilityStatusChanged(Item, SlotInputTag);
}
