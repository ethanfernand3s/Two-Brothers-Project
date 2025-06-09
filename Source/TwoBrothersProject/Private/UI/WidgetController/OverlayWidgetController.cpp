// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(ParasiteAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(ParasiteAttributeSet->GetMaxHealth());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	ParasiteASC->GetGameplayAttributeValueChangeDelegate(
		ParasiteAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	ParasiteASC->GetGameplayAttributeValueChangeDelegate(
		ParasiteAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(ParasiteAttributeSet->GetHealth());
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChanged.Broadcast(ParasiteAttributeSet->GetMaxHealth());
}
