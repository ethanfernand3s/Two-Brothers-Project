// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseGameplayAbility.h"

void UBaseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	if (HasAuthority(&ActivationInfo) && !IsLocallyControlled())
	{
		// If network mode variant is server (Listen Server, Dedicated Server) and we are on a remote client
		ActivateServerAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else
	{
		// If network mode variant is local player (Standalone, Client, Listen Server)
		ActivateLocalPlayerAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
}

void UBaseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	
	if (IsEndAbilityValid(Handle, ActorInfo))
	{
		if (ScopeLockCount == 0)
		{
			EndAbilityCleanup(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		}
		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}

void UBaseGameplayAbility::ActivateLocalPlayerAbility(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	// Override for functionality
}

void UBaseGameplayAbility::ActivateServerAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// Override for functionality
}

void UBaseGameplayAbility::EndAbilityCleanup(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	// Override for functionality
}
