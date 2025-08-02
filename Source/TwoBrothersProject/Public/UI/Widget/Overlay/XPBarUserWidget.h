// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "XPBarUserWidget.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UXPBarUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeStats",meta = (BindWidget))
	TObjectPtr<UTextBlock>  TextBlock_Level;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeStats",meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_XP;

protected:
	
	virtual void OnWidgetControllerSet() override;
};