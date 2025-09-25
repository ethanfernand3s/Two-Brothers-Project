// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "BaseProjectileAbility.generated.h"

class AProjectileActor;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UBaseProjectileAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

	protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;\

	UPROPERTY(editAnywhere,BlueprintReadOnly)
	TSubclassOf<AProjectileActor> ProjectileActor;
};
