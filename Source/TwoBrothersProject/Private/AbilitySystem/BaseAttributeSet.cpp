// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/BaseAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "TBGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/ParasitePlayerState.h"

UBaseAttributeSet::UBaseAttributeSet()
{
	const FTBGameplayTags& GameplayTags = FTBGameplayTags::Get();
	
	TagsToAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_Health,
		&UBaseAttributeSet::GetHealthAttribute,
		GameplayTags.Attributes_MaxHealth,
		&UBaseAttributeSet::GetMaxHealthAttribute
	));

	TagsToAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_Energy,
		&UBaseAttributeSet::GetEnergyAttribute,
		GameplayTags.Attributes_MaxEnergy,
		&UBaseAttributeSet::GetMaxEnergyAttribute
	));

	TagsToAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_Oxygen,
		&UBaseAttributeSet::GetOxygenAttribute,
		GameplayTags.Attributes_MaxOxygen,
		&UBaseAttributeSet::GetMaxOxygenAttribute
	));
	

	TagsToAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_Drowsiness,
		&UBaseAttributeSet::GetDrowsinessAttribute,
		GameplayTags.Attributes_MaxDrowsiness,
		&UBaseAttributeSet::GetMaxDrowsinessAttribute
	));

	TagsToAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_Strength,
		&UBaseAttributeSet::GetStrengthAttribute
	));

	TagsToAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_AuraStrength,
		&UBaseAttributeSet::GetAuraStrengthAttribute
	));

	TagsToAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_Defense,
		&UBaseAttributeSet::GetDefenseAttribute
	));

	TagsToAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_AuraDefense,
		&UBaseAttributeSet::GetAuraDefenseAttribute
	));

	TagsToAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_Speed,
		&UBaseAttributeSet::GetSpeedAttribute
	));
}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBaseAttributeSet, Health);
	DOREPLIFETIME(UBaseAttributeSet, MaxHealth);
	DOREPLIFETIME(UBaseAttributeSet, Type);
	DOREPLIFETIME(UBaseAttributeSet, Energy);
	DOREPLIFETIME(UBaseAttributeSet, MaxEnergy);
	DOREPLIFETIME(UBaseAttributeSet, Oxygen);
	DOREPLIFETIME(UBaseAttributeSet, MaxOxygen);
	DOREPLIFETIME(UBaseAttributeSet, Strength);
	DOREPLIFETIME(UBaseAttributeSet, AuraStrength);
	DOREPLIFETIME(UBaseAttributeSet, Defense);
	DOREPLIFETIME(UBaseAttributeSet, AuraDefense);
	DOREPLIFETIME(UBaseAttributeSet, Speed);
	DOREPLIFETIME(UBaseAttributeSet, Drowsiness);
	DOREPLIFETIME(UBaseAttributeSet, MaxDrowsiness);
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() - GetIncomingDamage(), 0, GetMaxHealth()));
		SetIncomingDamage(0.0f);
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

void UBaseAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Energy, OldValue);
}

void UBaseAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxEnergy, OldValue);
}

void UBaseAttributeSet::OnRep_Oxygen(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Oxygen, OldValue);
}

void UBaseAttributeSet::OnRep_MaxOxygen(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxOxygen, OldValue);
}

void UBaseAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Strength, OldValue);
}

void UBaseAttributeSet::OnRep_AuraStrength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, AuraStrength, OldValue);
}

void UBaseAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Defense, OldValue);
}

void UBaseAttributeSet::OnRep_AuraDefense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, AuraDefense, OldValue);
}

void UBaseAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Speed, OldValue);
}

void UBaseAttributeSet::OnRep_Drowsiness(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Drowsiness, OldValue);
}

void UBaseAttributeSet::OnRep_MaxDrowsiness(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxDrowsiness, OldValue);
}

void UBaseAttributeSet::OnRep_Type(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Type, OldValue);
}

float UBaseAttributeSet::CalculateCombatPower() const
{
	return (GetMaxHealth() + GetStrength() + GetDefense() + GetAuraStrength()+ GetAuraDefense() + GetSpeed());
}
