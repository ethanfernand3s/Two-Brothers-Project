// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityInputConfig.h"
#include "EnhancedInputComponent.h"
#include "TBInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOBROTHERSPROJECT_API UTBInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	template<class UserClass, typename PressedFunctionType, typename ReleasedFunctionType, typename HeldFunctionType>
	void BindAbilityActions(const UAbilityInputConfig* InputConfig, UserClass* Object, PressedFunctionType PressedFunction, ReleasedFunctionType ReleasedFunction, HeldFunctionType HeldFunction);
};

template <class UserClass, typename PressedFunctionType, typename ReleasedFunctionType, typename HeldFunctionType>
void UTBInputComponent::BindAbilityActions(const UAbilityInputConfig* InputConfig, UserClass* Object, PressedFunctionType PressedFunction, ReleasedFunctionType ReleasedFunction, HeldFunctionType HeldFunction)
{
	check(InputConfig);

	for (const FAbilityInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunction)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunction, Action.InputTag);
			}

			if (ReleasedFunction)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunction, Action.InputTag);
			}
			
			if (HeldFunction)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunction, Action.InputTag);
			}
		}
	}
}
