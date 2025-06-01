// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbilitySet.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseGameplayAbility.h"

void UBaseAbilitySet::GiveToAbilitySystem(UAbilitySystemComponent* ASC,TArray<FGameplayAbilitySpecHandle>* OutHandles) const
{
	if (!ASC) return;
	
	for (const FAbilitySet_Ability& A : Abilities)
	{
		if (A.AbilityClass)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(A.AbilityClass, A.AbilityLevel);
			{
				if (const UBaseGameplayAbility* BaseAbility = Cast<UBaseGameplayAbility>(AbilitySpec.Ability))
				{
					AbilitySpec.GetDynamicSpecSourceTags().AddTag(BaseAbility->StartupInputTag);
				}
				FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(AbilitySpec);
				if (OutHandles) OutHandles->Add(Handle);
			}
		}
	}
}
