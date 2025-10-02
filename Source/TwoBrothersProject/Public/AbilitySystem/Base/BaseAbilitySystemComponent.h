// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Inventory/Items/TBInventoryItem.h"
#include "BaseAbilitySystemComponent.generated.h"

class UBaseGameplayAbility;
struct FAbilitySet_Ability;
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPassiveEffectActivationChanged, const FGameplayTag& /*AbilityTag*/);

struct FCharacterCombatValues;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOBROTHERSPROJECT_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	FAbilitiesGiven AbilitiesGivenDelegate;
	FOnPassiveEffectActivationChanged ActivatePassiveEffect;
	FOnPassiveEffectActivationChanged DeactivatePassiveEffect;
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	// TODO: Remove, unnecessary
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	UFUNCTION(Server, Reliable)
	void Server_UpgradeAttribute(const FGameplayTag& AttributeTag);

	void HandleAbilityStatusChanged(const UTBInventoryItem* Item, FGameplayTag SlotInputTag);
	UFUNCTION(Server, Reliable)
	void Server_HandleAbilityStatusChanged(const UTBInventoryItem* Item, FGameplayTag SlotInputTag);
	FGameplayAbilitySpecHandle UnlockAbility(const TSubclassOf<UBaseGameplayAbility>& AbilityClass, int32 AbilityLevel,
	                   const FGameplayTag& NewStatus,
	                   const FGameplayTag& CreatureTypeTag);
	void EquipAbility(FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& SlotInputTag, bool bIsPassiveAbility);
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	bool SlotIsEmpty(const FGameplayTag& Slot);
	bool AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);
	bool AbilityHasAnySlot(const FGameplayAbilitySpec& Spec);
	FGameplayAbilitySpec* GetSpecWithSlot(const FGameplayTag& Slot);
	void AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);

	// RPC for directly calling from WC
	UFUNCTION(Server, Reliable)
	void Server_DirectUnEquipAbility(const UTBInventoryItem* Item);
	void UnEquipAbility(FGameplayAbilitySpec* AbilitySpec, bool bIsPassiveAbility);
	void ClearSlot(FGameplayAbilitySpec* Spec);
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag, bool bActivate);
	
	void AddCharacterAbility(const TSubclassOf<UGameplayAbility>& GameplayAbilityClass,
	                         int32 AbilityLevel, const FGameplayTag& InputTag);
	void AddCharacterPassiveAbility(const TSubclassOf<UGameplayAbility>& GameplayAbilityClass,
	int32 AbilityLevel);

	void ForEachAbility(const FForEachAbility& Delegate);
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	//static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);

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
