// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CompositeBase.h"
#include "Composite.generated.h"

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UComposite : public UCompositeBase
{
	GENERATED_BODY()
	
public:
	/* Adds all children in composite to array and hiding all the widgets. */
	virtual void NativeOnInitialized() override;

	/* Applies a desired function to all of its children. */
	virtual void ApplyFunction(FuncType Function) override;

	/* Hides all the children widgets. */
	virtual void Collapse() override;
	
	TArray<UCompositeBase*> GetChildren() { return Children; }
	
private:
	UPROPERTY()
	TArray<TObjectPtr<UCompositeBase>> Children;
};
