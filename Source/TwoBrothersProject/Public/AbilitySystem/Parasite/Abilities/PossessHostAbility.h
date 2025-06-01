// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BaseGameplayAbility.h"
#include "PossessHostAbility.generated.h"

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UPossessHostAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UPossessHostAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly)
	float LineTraceLength;
};
