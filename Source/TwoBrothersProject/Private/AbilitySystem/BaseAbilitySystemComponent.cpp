// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbilitySystemComponent.h"

#include "TBGameplayTags.h"
#include "AbilitySystem/Data/CharacterCombatValues.h"

void UBaseAbilitySystemComponent::AbilityActorInfoSet()
{
	
}

void UBaseAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				// TODO: Is Getting Primary Instance Of The Ability Spec Not All Instances, If This Becomes A Problem Fix It
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.GetPrimaryInstance()->GetCurrentActivationInfo().GetActivationPredictionKey());
			}
		}
	}
}

void UBaseAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UBaseAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			// TODO: Is Getting Primary Instance Of The Ability Spec Not All Instances, If This Becomes A Problem Fix It
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.GetPrimaryInstance()->GetCurrentActivationInfo().GetActivationPredictionKey());
		}
	}
}

void UBaseAbilitySystemComponent::SetBaseStats(float CurrentCombatPower, int32 Level)
{
	constexpr float Multiplier = 1.045f;              // Growth rate per level
	CurrentCombatPower *= FMath::Pow(Multiplier, Level - 1);
	
	// Generate wild procedural stats from CP
	TArray<float> RawWeights;
	for (int i = 0; i < 6; ++i)
	{
		RawWeights.Add(FMath::FRandRange(0.1f, 2.0f)); // More room for extreme bias
	}

	// Normalize weights
	float WeightSum = 0.f;
	for (float W : RawWeights) WeightSum += W;
	for (float& W : RawWeights) W /= WeightSum;

	// Convert to final stat values
	TArray<float> StatValues;
	int32 Allocated = 0;
	for (float W : RawWeights)
	{
		float Value = CurrentCombatPower * W;
		StatValues.Add(Value);
		Allocated += FMath::RoundToInt(Value);
	}

	// Distribute Remainder
	int32 Remainder = FMath::RoundToInt(CurrentCombatPower) - Allocated;
	while (Remainder > 0)
	{
		int Index = FMath::RandRange(0, 5);
		StatValues[Index] += 1.f;
		--Remainder;
	}

	// Assign to your combat stats
	const float Health        = StatValues[0];
	const float Strength      = StatValues[1];
	const float Defense       = StatValues[2];
	const float AuraStrength  = StatValues[3];
	const float AuraDefense   = StatValues[4];
	const float Speed         = StatValues[5];

	// Build GameplayEffectSpec
	FGameplayEffectSpecHandle SpecH =
		MakeOutgoingSpec(ApplyIvsGameplayEffect, 1.f, MakeEffectContext());

	FGameplayEffectSpec* Spec = SpecH.Data.Get();
	if (!Spec) return;

	const static FTBGameplayTags GameplayTags = FTBGameplayTags::Get();

	// Survival
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Type, 0.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Energy, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_MaxEnergy, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Oxygen, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_MaxOxygen, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Drowsiness, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_MaxDrowsiness, 100.f);

	// Combat
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Health, Health);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_MaxHealth, Health);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Strength, Strength);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_AuraStrength, AuraStrength);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Defense, Defense);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_AuraDefense, AuraDefense);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Speed, Speed);

	ApplyGameplayEffectSpecToSelf(*Spec);
}

void UBaseAbilitySystemComponent::SetBaseStats(const FCharacterCombatValues& CustomCombatValues)
{
	FGameplayEffectSpecHandle SpecH =
		MakeOutgoingSpec(ApplyIvsGameplayEffect, 1.f, MakeEffectContext());

	FGameplayEffectSpec* Spec = SpecH.Data.Get();
	if (!Spec) return;

	const static FTBGameplayTags GameplayTags = FTBGameplayTags::Get();

	// Survival
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Type, 0.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Energy, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_MaxEnergy, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Oxygen, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_MaxOxygen, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Drowsiness, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_MaxDrowsiness, 100.f);

	// Combat
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Health, CustomCombatValues.Health);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_MaxHealth, CustomCombatValues.Health);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Strength, CustomCombatValues.Strength);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_AuraStrength, CustomCombatValues.AuraStrength);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Defense, CustomCombatValues.Defense);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_AuraDefense, CustomCombatValues.AuraDefense);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Speed, CustomCombatValues.Speed);

	ApplyGameplayEffectSpecToSelf(*Spec);
}

void UBaseAbilitySystemComponent::AddIvsToAttributes(const FCharacterCombatValues& CharIvSet)
{
	FGameplayEffectSpecHandle SpecH =
		MakeOutgoingSpec(ApplyIvsGameplayEffect, 1.f, MakeEffectContext());

	FGameplayEffectSpec* Spec = SpecH.Data.Get();
	if (!Spec) return;

	const static FTBGameplayTags GameplayTags = FTBGameplayTags::Get();

	Spec->SetSetByCallerMagnitude(
		GameplayTags.Attributes_Health, CharIvSet.Health);
	Spec->SetSetByCallerMagnitude(
		GameplayTags.Attributes_MaxHealth, CharIvSet.Health);

	Spec->SetSetByCallerMagnitude(
		GameplayTags.Attributes_Strength, CharIvSet.Strength);
	Spec->SetSetByCallerMagnitude(
			GameplayTags.Attributes_AuraStrength, CharIvSet.AuraStrength);
	
	Spec->SetSetByCallerMagnitude(
		GameplayTags.Attributes_Defense, CharIvSet.Defense);
	Spec->SetSetByCallerMagnitude(
		GameplayTags.Attributes_AuraDefense, CharIvSet.AuraDefense);
	
	Spec->SetSetByCallerMagnitude(
		GameplayTags.Attributes_Speed, CharIvSet.Speed);
	
	
	ApplyGameplayEffectSpecToSelf(*Spec);
}
