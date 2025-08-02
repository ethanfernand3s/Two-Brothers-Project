// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BaseGameplayAbility.h"
#include "ListenForEventPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UListenForEventPassiveAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	
	UListenForEventPassiveAbility();

	UPROPERTY(EditAnywhere, Category=ClassVars)
	FGameplayTag AttributesTag;
	UPROPERTY(EditAnywhere, Category=ClassVars)
	TSubclassOf<UGameplayEffect> EventBasedEffectClass;
	
protected:
	
	virtual void ActivateServerAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload);
};
