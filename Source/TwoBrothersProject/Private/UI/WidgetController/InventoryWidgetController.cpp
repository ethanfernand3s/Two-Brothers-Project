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
#include "Player/TBPlayerController.h"
#include "UI/HUD/PlayerHUD.h"

void UInventoryWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	
	// Parasite
	if (ParasiteAS.IsValid())
	{
		BroadcastAllAttributes(ParasiteASC.Get(), ParasiteAS.Get(), true);
	}
	
	// Animal
	if (AnimalAS.IsValid())
	{
		BroadcastAllAttributes(AnimalASC.Get(), AnimalAS.Get(),false);
	}

	// Inventory
	if (TBPC.IsValid())
	{
		if (UTBInventoryComponent* Inv = TBPC->FindComponentByClass<UTBInventoryComponent>())
		{
			CachedInventory = Inv;
		}
	}
}

// Loop over all bindings in an AttributeSet and broadcast values
void UInventoryWidgetController::BroadcastAllAttributes(UAbilitySystemComponent* ASC, UBaseAttributeSet* AttributeSet, bool bIsParasiteVal) const
{
	for (const FTagAttributeBinding& Binding : AttributeSet->TagsToCombatAttributes)
	{
		if (Binding.HasSecondary())
		{
			BroadcastAttributePair(ASC, Binding, bIsParasiteVal);
		}
		else
		{
			BroadcastSingleAttribute(ASC, Binding, bIsParasiteVal);
		}
	}
}

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	
									/* Bind Character Context */
	// Parasite
	if (ParasiteASC.IsValid() && ParasiteAS.IsValid())
	{
		BindAllAttributeCallbacks(ParasiteASC.Get(), ParasiteAS.Get(), true);
	}
	
	// Animal
	if (AnimalASC.IsValid() && AnimalAS.IsValid())
	{
		BindAllAttributeCallbacks(AnimalASC.Get(), AnimalAS.Get(), false);
	}
	
}

void UInventoryWidgetController::UnBindCallbacks()
{
	Super::UnBindCallbacks();
	if (!AnimalAttributeHandles.IsEmpty()) return;
	for (const auto& KVP : AnimalAttributeHandles)
	{
		AnimalASC->GetGameplayAttributeValueChangeDelegate(KVP.Key).Remove(KVP.Value);
	}
	AnimalAttributeHandles.Empty();
	
	AnimalASC = nullptr;
	AnimalAS = nullptr;
	AnimalPI = nullptr;
}

// Loop over all bindings in an AttributeSet and bind callbacks
void UInventoryWidgetController::BindAllAttributeCallbacks(UAbilitySystemComponent* ASC, UBaseAttributeSet* AttributeSet,
														   bool bIsParasiteVal)
{
	for (const FTagAttributeBinding& Binding : AttributeSet->TagsToCombatAttributes)
	{
		BindAttributeCallback(ASC, Binding, bIsParasiteVal);
	}
}

// Bind attribute change callbacks for one binding
void UInventoryWidgetController::BindAttributeCallback( UAbilitySystemComponent* ASC,
														const FTagAttributeBinding& Binding,
														bool bIsParasiteVal)
{
	const FGameplayAttribute PrimaryAttribute = Binding.PrimaryAttributeFunc();

	// Bind Primary
	FDelegateHandle PrimaryHandle = ASC->GetGameplayAttributeValueChangeDelegate(PrimaryAttribute).AddLambda(
		[this, ASC, &Binding, bIsParasiteVal](const FOnAttributeChangeData& Data)
		{
			if (Binding.HasSecondary())
			{
				BroadcastAttributePair(ASC, Binding, bIsParasiteVal);
			}
			else
			{
				BroadcastSingleAttribute(ASC, Binding, bIsParasiteVal);
			}
		}
	);

	if (!bIsParasiteVal)
	{
		// Only track Animal handles
		AnimalAttributeHandles.Add(PrimaryAttribute, PrimaryHandle);
	}

	// Bind Secondary (if it exists)
	if (Binding.HasSecondary())
	{
		const FGameplayAttribute SecondaryAttribute = Binding.SecondaryAttributeFunc();

		FDelegateHandle SecondaryHandle = ASC->GetGameplayAttributeValueChangeDelegate(SecondaryAttribute).AddLambda(
			[this, ASC, &Binding, bIsParasiteVal](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributePair(ASC, Binding, bIsParasiteVal);
			}
		);

		if (!bIsParasiteVal)
		{
			AnimalAttributeHandles.Add(SecondaryAttribute, SecondaryHandle);
		}
	}
}

// Broadcast a Primary/Secondary attribute pair (Current + Max)
void UInventoryWidgetController::BroadcastAttributePair(UAbilitySystemComponent* ASC,
													    const FTagAttributeBinding& Binding, 
													    bool bIsParasiteVal) const
{
	FTBAttributeInfo CurrentInfo = AttributeInfoLibrary::FindAttributeInfo(Binding.PrimaryTag);
	CurrentInfo.AttributeValue = ASC->GetNumericAttribute(Binding.PrimaryAttributeFunc());

	FTBAttributeInfo MaxInfo = AttributeInfoLibrary::FindAttributeInfo(Binding.SecondaryTag);
	MaxInfo.AttributeValue = ASC->GetNumericAttribute(Binding.SecondaryAttributeFunc());

	CurrentAndMax_AttributeInfoDelegate.Broadcast(CurrentInfo, MaxInfo, bIsParasiteVal);
}

// Broadcast a single attribute (no secondary)
void UInventoryWidgetController::BroadcastSingleAttribute(UAbilitySystemComponent* ASC,
														  const FTagAttributeBinding& Binding, 
														  bool bIsParasiteVal) const
{
	FTBAttributeInfo Info = AttributeInfoLibrary::FindAttributeInfo(Binding.PrimaryTag);
	Info.AttributeValue = ASC->GetNumericAttribute(Binding.PrimaryAttributeFunc());

	Single_AttributeInfoDelegate.Broadcast(Info, bIsParasiteVal);
}

void UInventoryWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag) const
{
	UBaseAbilitySystemComponent* ActiveASC = GetActiveASC(!bIsParasiteFocusedCharacter);
	
	if (IsValid(ActiveASC)) ActiveASC->UpgradeAttribute(AttributeTag);
}

void UInventoryWidgetController::UpdateItemStatus(const UTBInventoryItem* Item, bool bIsEquippableSlot) const
{
	if (!CachedInventory.IsValid()) return;
	
	IPlayerInterface* ActivePI = GetActivePI(!bIsParasiteFocusedCharacter);
	if (!ActivePI) return;
	
	const UCharacterContextComponent* CharacterContextComp = ActivePI->GetCharacterContextComponent();
	if (!IsValid(CharacterContextComp)) return;

	// Illegal Otra Vez
	CachedInventory->Server_UpdateItemStatus(const_cast<UTBInventoryItem*>(Item), bIsEquippableSlot, CharacterContextComp);
}

void UInventoryWidgetController::HandleAbilityStatusChanged(const UTBInventoryItem* Item, FGameplayTag SlotInputTag)
{
	UBaseAbilitySystemComponent* ActiveASC = GetActiveASC(!bIsParasiteFocusedCharacter);
	if (IsValid(ActiveASC)) ActiveASC->HandleAbilityStatusChanged(Item, SlotInputTag);
}

void UInventoryWidgetController::UnEquipAbility(const UTBInventoryItem* Item)
{
	UBaseAbilitySystemComponent* ActiveASC = GetActiveASC(!bIsParasiteFocusedCharacter);
	if (IsValid(ActiveASC)) ActiveASC->Server_DirectUnEquipAbility(Item);
}
