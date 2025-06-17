// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Parasite/Abilities/Possessing/EnterHuntingModeAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "TBGameplayTags.h"


UEnterHuntingModeAbility::UEnterHuntingModeAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UEnterHuntingModeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo,
                                               const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    if (!CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }
    EnterHuntingMode();
}
void UEnterHuntingModeAbility::EnterHuntingMode()
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

    if (HuntingVisionEffect)
    {
        FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
        HuntingEffectSpec = MakeOutgoingGameplayEffectSpec(HuntingVisionEffect, 1.f);
        HuntingEffectSpec.Data->SetContext(Context);
        ASC->ApplyGameplayEffectSpecToSelf(*HuntingEffectSpec.Data);
    }
    
    ASC->ExecuteGameplayCue(FTBGameplayTags::Get().GameplayCue_State_Parasite_SeekingHost);
    ASC->AddReplicatedLooseGameplayTag(FTBGameplayTags::Get().State_Parasite_SeekingHost);

    
}

void UEnterHuntingModeAbility::ExitHuntingMode()
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    
    ASC->ExecuteGameplayCue(FTBGameplayTags::Get().GameplayCue_State_Parasite_FinishSeekingHost);
    ASC->RemoveLooseGameplayTag(FTBGameplayTags::Get().State_Parasite_SeekingHost);
    
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}