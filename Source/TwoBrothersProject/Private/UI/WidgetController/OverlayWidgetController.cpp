// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/Animal/AnimalAbilitySystemComponent.h"
#include "AbilitySystem/Animal/AnimalAttributeSet.h"
#include "Player/ParasitePlayerState.h"
#include "AbilitySystem/Base/BaseAttributeSet.h"
#include "AbilitySystem/Base/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
    Super::BroadcastInitialValues();
    
    // Parasite
    if (ParasiteAS.IsValid())
    {
        HealthChanged.Broadcast(ParasiteAS->GetHealth(), true);
        MaxHealthChanged.Broadcast(ParasiteAS->GetMaxHealth(), true);
        EnergyChanged.Broadcast(ParasiteAS->GetEnergy(), true);
        MaxEnergyChanged.Broadcast(ParasiteAS->GetMaxEnergy(), true);
    }

    // Animal
    if (AnimalAS.IsValid())
    {
        HealthChanged.Broadcast(AnimalAS->GetHealth(), false);
        MaxHealthChanged.Broadcast(AnimalAS->GetMaxHealth(), false);
        EnergyChanged.Broadcast(AnimalAS->GetEnergy(), false);
        MaxEnergyChanged.Broadcast(AnimalAS->GetMaxEnergy(), false);
    }
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
    // Parasite
    if (ParasiteASC.IsValid() && ParasiteAS.IsValid())
    {
        BindSelectedAttributes(ParasiteASC.Get(), ParasiteAS.Get(), true);
    }

    // Animal
    if (AnimalASC.IsValid() && AnimalAS.IsValid())
    {
        BindSelectedAttributes(AnimalASC.Get(), AnimalAS.Get(), false);
    }
}

void UOverlayWidgetController::BindSelectedAttributes(
    UAbilitySystemComponent* ASC,
    UBaseAttributeSet* AS,
    bool bIsParasiteVal)
{
    ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
        [this, bIsParasiteVal](const FOnAttributeChangeData& Data)
        {
            HealthChanged.Broadcast(Data.NewValue, bIsParasiteVal);
        });

    ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
        [this, bIsParasiteVal](const FOnAttributeChangeData& Data)
        {
            MaxHealthChanged.Broadcast(Data.NewValue, bIsParasiteVal);
        });

    ASC->GetGameplayAttributeValueChangeDelegate(AS->GetEnergyAttribute()).AddLambda(
        [this, bIsParasiteVal](const FOnAttributeChangeData& Data)
        {
            EnergyChanged.Broadcast(Data.NewValue, bIsParasiteVal);
        });

    ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxEnergyAttribute()).AddLambda(
        [this, bIsParasiteVal](const FOnAttributeChangeData& Data)
        {
            MaxEnergyChanged.Broadcast(Data.NewValue, bIsParasiteVal);
        });
}
