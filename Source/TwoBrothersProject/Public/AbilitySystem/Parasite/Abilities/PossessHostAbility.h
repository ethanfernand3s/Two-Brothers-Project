// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BaseGameplayAbility.h"
#include "PossessHostAbility.generated.h"

class APossessMiniGame;
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
	
	void OnPossessResultReceived(bool bSuccess);

	UFUNCTION(Server, Reliable)
	void Server_OnPossessResultReceived(bool bSuccess);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APossessMiniGame> PossessMiniGameClass;
	UPROPERTY(EditDefaultsOnly)
	float LineTraceLength;
};
