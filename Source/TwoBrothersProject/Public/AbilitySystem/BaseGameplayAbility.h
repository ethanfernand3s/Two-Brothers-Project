// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

/**
 *	Base gameplay ability that all abilities in the project must inherit from.
 *
 *	                                      
 *  - To carry out CLIENT implementation of the ability override ActivateLocalPlayerAbility.
 *	- To carry out SERVER implementation of the ability override ActivateServerAbility.
 *	- To clean up dynamic data override EndAbilityCleanup.
 *	
 *	@remark MUST set child abilities startup input tag.
 */
UCLASS()
class TWOBROTHERSPROJECT_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,  Category = "input")
	FGameplayTag StartupInputTag;
	
protected:
	//~UGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, //check for server or local
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End of UGameplayAbility interface
	
	/* Override to carry out CLIENT implementation of the ability */
	virtual void ActivateLocalPlayerAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	/* Override to carry out SERVER implementation of the ability */
	virtual void ActivateServerAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	/* Override to clean-up anything dynamic on the ending of the ability */
	virtual void EndAbilityCleanup(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
};
