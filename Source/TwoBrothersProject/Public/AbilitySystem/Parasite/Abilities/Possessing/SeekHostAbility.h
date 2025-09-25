// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Base/Abilities/BaseGameplayAbility.h"
#include "SeekHostAbility.generated.h"

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API USeekHostAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
public:
	USeekHostAbility();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbilityCleanup(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UPROPERTY()
	FActiveGameplayEffectHandle HuntingEffectHandle;
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> HuntingVisionEffect;

	UPROPERTY()
	FGameplayEffectSpecHandle HuntingEffectSpec;
	
	void EnterHuntingMode();
	void ExitHuntingMode();
};