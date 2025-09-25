// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Base/BaseAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TBGameplayTags.h"
#include "AbilitySystem/Base/BaseAbilitySet.h"
#include "AbilitySystem/Base/Abilities/BaseGameplayAbility.h"
#include "AbilitySystem/Data/CharacterCombatValues.h"
#include "Characters/Components/CharacterContextComponent.h"
#include "Player/Interfaces/PlayerInterface.h"

void UBaseAbilitySystemComponent::HandleAbilityItemEquipped(const UTBInventoryItem* Item, FGameplayTag SlotInputTag)
{
}

void UBaseAbilitySystemComponent::AddCharacterAbility(const TSubclassOf<UBaseGameplayAbility>& GameplayAbilityClass,
                                                      int32 AbilityLevel, const FGameplayTag& InputTag)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GameplayAbilityClass, AbilityLevel);
	AbilitySpec.GetDynamicSpecSourceTags().AddTag(InputTag);
	GiveAbility(AbilitySpec);
	
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UBaseAbilitySystemComponent::AddCharacterPassiveAbility(const TSubclassOf<UBaseGameplayAbility>& GameplayAbilityClass,
	int32 AbilityLevel)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GameplayAbilityClass, AbilityLevel);
	GiveAbilityAndActivateOnce(AbilitySpec);
}

void UBaseAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UBaseAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->GetAssetTags())
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}


FGameplayTag UBaseAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Inputs"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

// Commenting for now since the item will keep track of this
// FGameplayTag UBaseAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
// {
// 	for (FGameplayTag StatusTag : AbilitySpec.GetDynamicSpecSourceTags())
// 	{
// 		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
// 		{
// 			return StatusTag;
// 		}
// 	}
// 	return FGameplayTag();
// }

// TODO: Change these input functions to come from an item, therefore we can't check the AbilitySpec's
// Tag since it will be on the items Input Fragment
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

void UBaseAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	ServerUpgradeAttribute(AttributeTag);
}

void UBaseAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	AActor* CurrentAvatarActor = GetAvatarActor();
	if (CurrentAvatarActor && CurrentAvatarActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(CurrentAvatarActor);

		if (PlayerInterface && PlayerInterface->GetCharacterContextComponent()->GetAttributePoints() > 0)
		{
			// Send Gameplay Event
			FGameplayEventData Payload;
			Payload.EventTag = AttributeTag;
			Payload.EventMagnitude = 1.f;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CurrentAvatarActor, AttributeTag, Payload);

			// Subtract Attribute Point
			PlayerInterface->GetCharacterContextComponent()->AddToAttributePoints(-1);
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

	// Assign combat stats
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

	// Survival Stats
	// TODO: Change later to have these be different with each animal
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Energy, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_MaxEnergy, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Oxygen, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_MaxOxygen, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Drowsiness, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_MaxDrowsiness, 100.f);

	// Combat Stats
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

	// Combat Stats
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

void UBaseAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}