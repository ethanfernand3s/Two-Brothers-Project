#include "AbilitySystem/Parasite/Abilities/Possessing/SeekHostAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

USeekHostAbility::USeekHostAbility()
{
	InstancingPolicy   = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void USeekHostAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo,
                                               const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bRep=*/true, /*bWasCancelled=*/false);
		return;
	}
	
	EnterHuntingMode();
}

void USeekHostAbility::EnterHuntingMode()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC || !HuntingVisionEffect)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
				   /*bRep=*/true, /*bCancelled=*/true);
		return;
	}
	
	FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle     SpecHandle =
		ASC->MakeOutgoingSpec(HuntingVisionEffect, GetAbilityLevel(), Ctx);

	HuntingEffectHandle = ApplyGameplayEffectSpecToOwner(CurrentSpecHandle,
								   CurrentActorInfo,
								   CurrentActivationInfo,
								   SpecHandle);
}

void USeekHostAbility::ExitHuntingMode()
{
	if (!HuntingEffectHandle.IsValid()) return;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC) return; 

	ASC->RemoveActiveGameplayEffect(HuntingEffectHandle);

	HuntingEffectHandle.Invalidate();
}

void USeekHostAbility::EndAbilityCleanup(const FGameplayAbilitySpecHandle /*Handle*/,
                                                 const FGameplayAbilityActorInfo* /*ActorInfo*/,
                                                 const FGameplayAbilityActivationInfo /*ActivationInfo*/,
                                                 bool /*bReplicateEndAbility*/,
                                                 bool /*bWasCancelled*/)
{
	ExitHuntingMode();
}
