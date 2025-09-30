// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "OverlayAbilitySlotContainers.generated.h"

class USlotContainerUserWidget;
/**
 * TODO: Implement necessary changes, in its current state this won't even work.
 * Slots must directly correlate with indices broadcast from server
 */
UCLASS()
class TWOBROTHERSPROJECT_API UOverlayAbilitySlotContainers : public UBaseUserWidget
{
	GENERATED_BODY()
	
	// TODO: Convert these to lightweight slot/item holder that is meant for display only not logic,
	// then have inventory inherit and use the same but with additional functionality
	UPROPERTY(meta = (BindWidget))
	USlotContainerUserWidget* MainAbility_SlotContainer;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	USlotContainerUserWidget* DefaultAbilities_SlotContainer;
};
