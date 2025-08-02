// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

struct FAbilitySet_Ability;
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);

struct FCharacterCombatValues;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOBROTHERSPROJECT_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	FAbilitiesGiven AbilitiesGivenDelegate;
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	void AddCharacterAbilities(const TArray<FAbilitySet_Ability>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<FAbilitySet_Ability>& StartupPassiveAbilities);

	void ForEachAbility(const FForEachAbility& Delegate);
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	// TODO: Move calculating logic to procedural creature spawner
	/**
	 * The SetBaseStats function takes a creature’s CurrentCombatPower and procedurally splits it into six randomized stats
	 * (Health, Strength, Defense, AuraStrength, AuraDefense, Speed) using normalized random weights. These stats are applied
	 *  via a gameplay effect alongside survival stats.
	 *
	 *  @attention NEED TO MOVE TO PROCEDURAL CREATURE SPAWNER !
	 */
	void SetBaseStats(float CurrentCombatPower, int32 Level);
	/** Takes desired combat values and applies it via a gameplay effect alongside survival stats. **/
	void SetBaseStats(const FCharacterCombatValues& CharIvSet);
	
	void AddIvsToAttributes(const FCharacterCombatValues& CharIvSet);

	bool bStartupAbilitiesGiven = false;
	
protected:

	virtual void OnRep_ActivateAbilities() override;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> ApplyIvsGameplayEffect;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> BaseStatsGameplayEffect;
	
};
