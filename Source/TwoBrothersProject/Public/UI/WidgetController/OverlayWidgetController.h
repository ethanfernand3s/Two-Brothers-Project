// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.h"
#include "AbilitySystem/Base/BaseAttributeSet.h"
#include "OverlayWidgetController.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributePointsChanged, int32, bool bIsParasiteVal);
DECLARE_MULTICAST_DELEGATE_OneParam(FInt32DelegateSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FFloatDelegateSignature, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FTextureDelegateSignature, UTexture2D*, bool bIsParasiteVal);

UCLASS()
class TWOBROTHERSPROJECT_API UOverlayWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()
	
public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	void BindSelectedAttributes(UAbilitySystemComponent* ASC, UBaseAttributeSet* AS);

	// TODO: Refactor for both animal and parasite
	FOnAttributePointsChanged AttributePointsChanged;
	FInt32DelegateSignature LevelChanged;
	FFloatDelegateSignature XPChanged;
	
	FFloatDelegateSignature HealthChanged;
	FFloatDelegateSignature MaxHealthChanged;
	FFloatDelegateSignature EnergyChanged;
	FFloatDelegateSignature MaxEnergyChanged;

	FTextureDelegateSignature CharacterIconChanged;
};
