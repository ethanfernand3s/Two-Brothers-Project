// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.h"
#include "InventoryWidgetController.generated.h"


struct FTBAttributeInfo;
// For broadcasting both current and max values (e.g., Health / MaxHealth)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributeCurrentAndMaxChangedSignature, const FTBAttributeInfo&, const FTBAttributeInfo&);
// For broadcasting a single value (e.g., Strength, Speed)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChangedSignature, const FTBAttributeInfo&);

UCLASS()
class TWOBROTHERSPROJECT_API UInventoryWidgetController : public UBaseWidgetController
{

	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	FOnAttributeCurrentAndMaxChangedSignature CurrentAndMax_AttributeInfoDelegate;
	FOnAttributeValueChangedSignature Single_AttributeInfoDelegate;
	FOnAttributeValueChangedSignature Type_AttributeInfoDelegate;
};