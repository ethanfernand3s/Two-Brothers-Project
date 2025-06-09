// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChangedSignature_DEPRECATED, float);
UCLASS()
class TWOBROTHERSPROJECT_API UOverlayWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()
	
public:
	
	FOnAttributeValueChangedSignature_DEPRECATED OnHealthChanged;
	FOnAttributeValueChangedSignature_DEPRECATED OnMaxHealthChanged;

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

private:
	void HealthChanged(const FOnAttributeChangeData& Data);
	void MaxHealthChanged(const FOnAttributeChangeData& Data);
};
