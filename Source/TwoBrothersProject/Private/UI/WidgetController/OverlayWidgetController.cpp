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
        HealthChanged.Broadcast(
            ParasiteASC->GetNumericAttribute(ParasiteAS->GetHealthAttribute()), true);
        MaxHealthChanged.Broadcast(
            ParasiteASC->GetNumericAttribute(ParasiteAS->GetMaxHealthAttribute()), true);
        AuraChanged.Broadcast(
            ParasiteASC->GetNumericAttribute(ParasiteAS->GetAuraAttribute()), true);
        MaxAuraChanged.Broadcast(
            ParasiteASC->GetNumericAttribute(ParasiteAS->GetMaxAuraAttribute()), true);
    }

    // Animal
    if (AnimalAS.IsValid())
    {
        HealthChanged.Broadcast(
            AnimalASC->GetNumericAttribute(AnimalAS->GetHealthAttribute()), false);
        MaxHealthChanged.Broadcast(
            AnimalASC->GetNumericAttribute(AnimalAS->GetMaxHealthAttribute()), false);
        AuraChanged.Broadcast(
            AnimalASC->GetNumericAttribute(AnimalAS->GetAuraAttribute()), false);
        MaxAuraChanged.Broadcast(
            AnimalASC->GetNumericAttribute(AnimalAS->GetMaxAuraAttribute()), false);
    }
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
    Super::BindCallbacksToDependencies();
    
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

    ASC->GetGameplayAttributeValueChangeDelegate(AS->GetAuraAttribute()).AddLambda(
        [this, bIsParasiteVal](const FOnAttributeChangeData& Data)
        {
            AuraChanged.Broadcast(Data.NewValue, bIsParasiteVal);
        });

    ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxAuraAttribute()).AddLambda(
        [this, bIsParasiteVal](const FOnAttributeChangeData& Data)
        {
            MaxAuraChanged.Broadcast(Data.NewValue, bIsParasiteVal);
        });
}
