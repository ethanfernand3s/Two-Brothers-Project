// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "StatusBarUserWidget.generated.h"

class UAttributeSet;
class UBaseAttributeSet;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UStatusBarUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_HealthBar;
	
protected:

	/*
	void OnHealthChanged(float NewHealth);
	void OnMaxHealthChanged(float NewMaxHealth);
	*/

	UPROPERTY()
	UBaseAttributeSet* AttributeSet = nullptr;

private:
	
	UFUNCTION()
	void RefreshHealth();
};
