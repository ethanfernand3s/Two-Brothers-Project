// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUserWidget.h"
#include "OverlayUserWidget.generated.h"

class UTextBlock;
class UImage;
class UProgressBar;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UOverlayUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:

#pragma region ProgressBars
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Health;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Stamina;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Hunger;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Thirst;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Drowsiness;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Oxygen;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Temperature;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_XP;

#pragma endregion ProgressBars

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Type;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Level;
	
protected:
	
	virtual void OnWidgetControllerSet() override;

private:

	void OnHealthChanged(float NewHealth);
	void OnMaxHealthChanged(float NewMaxHealth);
	void SetHealthBarPercentage();
	UPROPERTY()
	float Health;
	UPROPERTY()
	float MaxHealth;
};
