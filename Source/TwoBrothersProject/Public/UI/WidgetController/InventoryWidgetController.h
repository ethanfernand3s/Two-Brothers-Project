// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.h"
#include "InventoryWidgetController.generated.h"


struct FTBCreatureTypeAttributeInfo;
struct FTBFloatAttributeInfo;

// For broadcasting both current and max values (e.g., Health / MaxHealth)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributeCurrentAndMaxChangedSignature, const FTBFloatAttributeInfo&, const FTBFloatAttributeInfo&);
// For broadcasting a single value (e.g., Strength, Speed)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChangedSignature, const FTBFloatAttributeInfo&);
// For broadcasting creature type changes (e.g., Fire, Water)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreatureTypeChangedSignature, const FTBCreatureTypeAttributeInfo&);

UCLASS()
class TWOBROTHERSPROJECT_API UInventoryWidgetController : public UBaseWidgetController
{

	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	FOnAttributeCurrentAndMaxChangedSignature Health_AttributeInfoDelegate;
};