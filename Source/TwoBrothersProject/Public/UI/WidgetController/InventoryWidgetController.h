// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.h"
#include "Characters/Data/BiomeDataAsset.h"
#include "Characters/Data/TribeData.h"
#include "InventoryWidgetController.generated.h"


struct FTBAttributeInfo;
// Attribute Delegates
// For broadcasting both current and max values (e.g., Health / MaxHealth)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributeCurrentAndMaxChangedSignature, const FTBAttributeInfo&, const FTBAttributeInfo&);
// For broadcasting a single value (e.g., Strength, Speed)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChangedSignature, const FTBAttributeInfo&);

// Character Context Delegates
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFloatValueChangedSignature, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttributePointsChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBiomeChangedSignature, const UBiomeDataAsset*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterNameChangedSignature, const FText&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTribeDataChangedSignature, const FTribeData&);

UCLASS()
class TWOBROTHERSPROJECT_API UInventoryWidgetController : public UBaseWidgetController
{

	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	void OnXPChanged(int NewXP);
	void OnAttributePointsChanged(int NewAttributePoints);
	void OnBiomeChanged(const UBiomeDataAsset* NewBiomeData);
	void OnCharacterNameChanged(const FText& NewCharacterName);
	void OnLevelChanged(int NewLevel);
	void OnTribeDataChanged(const FTribeData& NewTribeData);
	
	FOnAttributeCurrentAndMaxChangedSignature CurrentAndMax_AttributeInfoDelegate;
	FOnAttributeValueChangedSignature Single_AttributeInfoDelegate;
	FOnAttributeValueChangedSignature Type_AttributeInfoDelegate;
	
	FOnFloatValueChangedSignature OnXpPercentChangedDelegate;
	FOnAttributePointsChangedSignature OnAttributePointsChangedDelegate;
	FOnBiomeChangedSignature OnBiomeChangedDelegate;
	FOnCharacterNameChangedSignature OnCharacterNameChangedDelegate;
	FOnLevelChangedSignature OnLevelChangedDelegate;
	FOnTribeDataChangedSignature OnTribeDataChangedDelegate;
};