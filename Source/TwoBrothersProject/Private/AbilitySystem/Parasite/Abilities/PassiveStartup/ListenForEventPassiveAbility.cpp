// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Parasite/Abilities/PassiveStartup/ListenForEventPassiveAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UListenForEventPassiveAbility::UListenForEventPassiveAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UListenForEventPassiveAbility::ActivateServerAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!AttributesTag.IsValid()) EndAbility( Handle, ActorInfo, ActivationInfo, false, false);
	
    UAbilityTask_WaitGameplayEvent* WaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
        this,                    
        AttributesTag,          
        nullptr,                  
        false,                    
        false                     
    );
    
    if (WaitTask)
    {
        WaitTask->EventReceived.AddDynamic(this, &UListenForEventPassiveAbility::OnGameplayEventReceived);
        WaitTask->Activate();
    }
}


void UListenForEventPassiveAbility::OnGameplayEventReceived(FGameplayEventData Payload)
{
    UE_LOG(LogTemp, Warning, TEXT("%s: Gameplay Event Received! Tag: %s"), *GetNameSafe(this), *Payload.EventTag.ToString());

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (ASC)
	{
		FGameplayEffectSpecHandle GESpecHandle = ASC->MakeOutgoingSpec(EventBasedEffectClass, 1, ASC->MakeEffectContext());
		GESpecHandle.Data->SetSetByCallerMagnitude(Payload.EventTag, Payload.EventMagnitude);

		ASC->ApplyGameplayEffectSpecToSelf(*GESpecHandle.Data);
	}
	
}
