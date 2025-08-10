// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "ItemsPanelUserWidget.generated.h"

class UAbilityCardUserWidget;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UItemsPanelUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:

	void ReceiveNewCard(const UAbilityCardUserWidget* AbilityCard);
};
