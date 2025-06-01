// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusBarUserWidget.generated.h"

class UAttributeSet;
class UBaseAttributeSet;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UStatusBarUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UFUNCTION()
	void SetAttributeSet(UBaseAttributeSet* InAttributeSet);

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_HealthBar;

private:
	
	UFUNCTION()
	void RefreshHealth();
	
protected:
	
	UFUNCTION()
	void OnHealthChanged(const UAttributeSet* InAttributeSet, float OldValue, float NewValue);

	UPROPERTY()
	UBaseAttributeSet* AttributeSet = nullptr;
};
