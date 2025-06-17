// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BaseGameplayAbility.h"
#include "EnterHuntingModeAbility.generated.h"

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UEnterHuntingModeAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
public:
	UEnterHuntingModeAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
							   const FGameplayAbilityActorInfo* ActorInfo,
							   const FGameplayAbilityActivationInfo ActivationInfo,
							   const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> HuntingVisionEffect;
	
	FGameplayEffectSpecHandle HuntingEffectSpec;
	
	void EnterHuntingMode();
	void ExitHuntingMode();
};
