// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.h"
#include "AbilitySystem/Base/BaseAttributeSet.h"
#include "Inventory/Items/TB_ItemFragments.h"
#include "UI/Widget/Inventory/Slots/SlotContainerUserWidget.h"
#include "InventoryWidgetController.generated.h"

class UCharacterContextComponent;
class UTBInventoryItem;
class APlayerHUD;
class UTBInventoryComponent;

struct FGameplayTag;
struct FTBAttributeInfo;

// Attribute Delegates
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAttributeCurrentAndMaxChangedSignature, const FTBAttributeInfo&, const FTBAttributeInfo&, bool bIsParasiteVal);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributeValueChangedSignature, const FTBAttributeInfo&, bool bIsParasiteVal);

UCLASS()
class TWOBROTHERSPROJECT_API UInventoryWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	virtual void UnBindCallbacks() override;
	
	// GAS attr upgrades
	void UpgradeAttribute(const FGameplayTag& AttributeTag) const;
	void TryUnlockItem(UTBInventoryItem* Item, bool bIsEquippableSlot) const;
	// TODO: Change to set Grid Status and make each grid have grid data like slotted items have item data
	// void TryUnlockGridSlot(UInventoryGridSlot* GridSlot) const; // TODO: Implement
	void HandleAbilityStatusChanged(const UTBInventoryItem* Item, FGameplayTag SlotInputTag); // TODO: Move to equipment component
	
	// Attribute Set Delegates
	FOnAttributeCurrentAndMaxChangedSignature CurrentAndMax_AttributeInfoDelegate;
	FOnAttributeValueChangedSignature         Single_AttributeInfoDelegate;

private:

									/* Broadcast and Bind Helpers */

	/* Attribute Set */
	void BroadcastAllAttributes(UBaseAttributeSet* AttributeSet, bool bIsParasiteVal) const;
	void BindAllAttributeCallbacks(UAbilitySystemComponent* ASC, UBaseAttributeSet* AttributeSet,
								   bool bIsParasiteVal);
	void BindAttributeCallback(UAbilitySystemComponent* ASC, const FTagAttributeBinding& Binding,
							   UBaseAttributeSet* AttributeSet, bool bIsParasiteVal);
	void BroadcastAttributePair(const FTagAttributeBinding& Binding, UBaseAttributeSet* AttributeSet,
								bool bIsParasiteVal) const;
	void BroadcastSingleAttribute(const FTagAttributeBinding& Binding, UBaseAttributeSet* AttributeSet,
								  bool bIsParasiteVal) const;
	
	TWeakObjectPtr<UTBInventoryComponent> CachedInventory;
	bool bIsParasiteFocusedCharacter = true;
};
