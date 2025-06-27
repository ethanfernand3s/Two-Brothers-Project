// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"


struct FCharacterCombatValues;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOBROTHERSPROJECT_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	/**
	 * The SetBaseStats function takes a creature’s CurrentCombatPower and procedurally splits it into six randomized stats
	 * (Health, Strength, Defense, AuraStrength, AuraDefense, Speed) using normalized random weights. These stats are applied
	 *  via a gameplay effect alongside survival stats.
	 */
	void SetBaseStats(float CurrentCombatPower, int32 Level);
	
	/** Takes desired combat values and applies it via a gameplay effect alongside survival stats. **/
	void SetBaseStats(const FCharacterCombatValues& CharIvSet);
	
	void AddIvsToAttributes(const FCharacterCombatValues& CharIvSet);

protected:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> ApplyIvsGameplayEffect;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> BaseStatsGameplayEffect;
};
