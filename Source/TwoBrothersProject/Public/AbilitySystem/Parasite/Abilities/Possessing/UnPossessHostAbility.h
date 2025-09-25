// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Base/Abilities/BaseGameplayAbility.h"
#include "UnPossessHostAbility.generated.h"

class AParasiteCharacter;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UUnPossessHostAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UUnPossessHostAbility();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AParasiteCharacter> ParasiteCharacterClass;

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
