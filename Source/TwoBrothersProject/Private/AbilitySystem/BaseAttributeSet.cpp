// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/BaseAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/ParasitePlayerState.h"

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBaseAttributeSet, Health);
	DOREPLIFETIME(UBaseAttributeSet, MaxHealth);
	DOREPLIFETIME(UBaseAttributeSet, Type);
	DOREPLIFETIME(UBaseAttributeSet, Level);
	DOREPLIFETIME(UBaseAttributeSet, Stamina);
	DOREPLIFETIME(UBaseAttributeSet, MaxStamina);
	DOREPLIFETIME(UBaseAttributeSet, Oxygen);
	DOREPLIFETIME(UBaseAttributeSet, MaxOxygen);
	DOREPLIFETIME(UBaseAttributeSet, Hunger);
	DOREPLIFETIME(UBaseAttributeSet, MaxHunger);
	DOREPLIFETIME(UBaseAttributeSet, Thirst);
	DOREPLIFETIME(UBaseAttributeSet, MaxThirst);
	DOREPLIFETIME(UBaseAttributeSet, Strength);
	DOREPLIFETIME(UBaseAttributeSet, Speed);
	DOREPLIFETIME(UBaseAttributeSet, TemperatureResistance);
	DOREPLIFETIME(UBaseAttributeSet, Drowsiness);
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), 0, GetMaxHealth()));
		SetDamage(0.0f);
	}
}

void UBaseAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	UE_LOG(LogTemp, Warning, TEXT("PostChange: Attribute '%s' changed %.2f -> %.2f"), *Attribute.AttributeName, OldValue, NewValue);
	
	if (Attribute == GetMaxHealthAttribute())
	{
		if (GetHealth() > NewValue)
		{
			UBaseAbilitySystemComponent* TBAbilitySystem = GetTBAbilitySystemComponent();
			check(TBAbilitySystem);
			TBAbilitySystem->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
		}
	}
}

UBaseAbilitySystemComponent* UBaseAttributeSet::GetTBAbilitySystemComponent() const
{
	AParasitePlayerState* TBPlayerState = Cast<AParasitePlayerState>(GetOwningActor());
	if (TBPlayerState)
	{
		return Cast<UBaseAbilitySystemComponent>(TBPlayerState->GetAbilitySystemComponent());
	}
	return nullptr;
}

void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldValue);
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldValue);
}

void UBaseAttributeSet::OnRep_Level(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Level, OldValue);
}

void UBaseAttributeSet::OnRep_TemperatureResistance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Level, OldValue);
}

void UBaseAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Stamina, OldValue);
}

void UBaseAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxStamina, OldValue);
}

void UBaseAttributeSet::OnRep_Oxygen(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Oxygen, OldValue);
}

void UBaseAttributeSet::OnRep_MaxOxygen(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxOxygen, OldValue);
}

void UBaseAttributeSet::OnRep_Hunger(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Hunger, OldValue);
}

void UBaseAttributeSet::OnRep_MaxHunger(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHunger, OldValue);
}

void UBaseAttributeSet::OnRep_Thirst(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Thirst, OldValue);
}

void UBaseAttributeSet::OnRep_MaxThirst(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxThirst, OldValue);
}

void UBaseAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Strength, OldValue);
}

void UBaseAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Defense, OldValue);
}

void UBaseAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Speed, OldValue);
}

void UBaseAttributeSet::OnRep_Drowsiness(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Drowsiness, OldValue);
}

void UBaseAttributeSet::OnRep_Type(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Type, OldValue);
}

