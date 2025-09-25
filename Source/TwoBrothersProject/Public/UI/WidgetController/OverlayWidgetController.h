// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInt32DelegateSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FFloatDelegateSignature, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FTextureDelegateSignature, UTexture*);

UCLASS()
class TWOBROTHERSPROJECT_API UOverlayWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()
	
public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	// TODO: Refactor for both animal and parasite
	FInt32DelegateSignature AttributePointsChanged;
	FInt32DelegateSignature LevelChanged;
	FFloatDelegateSignature XPChanged;
	
	FFloatDelegateSignature HealthChanged;
	FFloatDelegateSignature MaxHealthChanged;
	FFloatDelegateSignature EnergyChanged;
	FFloatDelegateSignature MaxEnergyChanged;

	// FTextureDelegateSignature IconChanged; // Leave this out for now
};
