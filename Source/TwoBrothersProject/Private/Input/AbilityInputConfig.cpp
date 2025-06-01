// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AbilityInputConfig.h"

const UInputAction* UAbilityInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputAbilityTag, bool bLogNotFound) const
{
	for (const FAbilityInputAction& Action: AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputAbilityTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputAbilityTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}