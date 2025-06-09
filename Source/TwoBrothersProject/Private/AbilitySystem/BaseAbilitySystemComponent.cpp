// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbilitySystemComponent.h"

#include "TBGameplayTags.h"
void UBaseAbilitySystemComponent::AbilityActorInfoSet()
{
	const FTBGameplayTags& GameplayTags = FTBGameplayTags::Get();

	GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Orange,FString::Printf(TEXT("dadaddadadyyyyy %s"),*GameplayTags.InputTag_E.ToString()));
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
