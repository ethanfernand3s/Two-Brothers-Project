// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "BaseGameplayAbility_ClientToServer.generated.h"

/**
 * Base ability for client to server interactions (Abilities that require client data for server actions).
 * @remark MUST override ActivateAbilityWithTargetData for client to server communication.
 * @remark ActivateLocalPlayerAbility must call NotifyTargetDataReady after the target data is ready
 */
UCLASS()
class TWOBROTHERSPROJECT_API UBaseGameplayAbility_ClientToServer : public UBaseGameplayAbility
{
	GENERATED_BODY()
public:
	/* This runs on the local player client as prediction code AND on the server as authoritative code after the TargetData is received from the client */
	virtual void ActivateAbilityWithTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle, FGameplayTag ApplicationTag);
	
protected:
	//~UBaseGameplayAbility interface
	virtual void ActivateServerAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbilityCleanup(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UBaseGameplayAbility interface
	
	/* Sends the client-computed TargetData to the server. */ 
	virtual void NotifyTargetDataReady(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	// Delegate for notifying ability that target data is ready.
	FDelegateHandle NotifyTargetDataReadyDelegateHandle;
};
