// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/Widget/BaseUserWidget.h"
#include "CompositeBase.generated.h"

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UCompositeBase : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:

	/* Hides the widget */
	virtual void Collapse();

	/* Shows the widget */
	void Expand();
	
	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(const FGameplayTag& Tag) { FragmentTag = Tag; }
	
	using FuncType = TFunction<void(UCompositeBase*)>;
	virtual void ApplyFunction(FuncType Function) {}
	
private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag FragmentTag;
};
