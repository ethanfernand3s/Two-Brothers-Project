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

protected:
    void PerformPossessionTrace();
    void StartMiniGame(AActor* TargetAnimal, FName ClosestSocket);
    void OnMiniGameCompleted(bool bSuccess, float ExecutionBonus);
    
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<APossessMiniGame> PossessMiniGameClass;

    UPROPERTY(EditDefaultsOnly)
    float SocketHitRange = 100.f;
};