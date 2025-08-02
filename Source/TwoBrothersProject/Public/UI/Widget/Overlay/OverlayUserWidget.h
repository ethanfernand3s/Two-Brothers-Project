// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseUserWidget.h"
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
	TObjectPtr<UProgressBar> ProgressBar_Energy;
	

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_XP;

#pragma endregion ProgressBars

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Level;
	
protected:
	
	virtual void OnWidgetControllerSet() override;
};
