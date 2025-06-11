// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "PossessionChanceUserWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UPossessionChanceUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()
public:
	void SetPossessionChance(float InChance);
	
	private:
	UPROPERTY(EditAnywhere,Category= "PossessionChance",meta = (BindWidget))
	TObjectPtr<UTextBlock> PossessionChance;

	
	
};
