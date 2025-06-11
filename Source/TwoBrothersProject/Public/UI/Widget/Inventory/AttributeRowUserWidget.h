// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "UI/Widget/BaseUserWidget.h"
#include "AttributeRowUserWidget.generated.h"


class UButton;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UAttributeRowUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> AttributeGameplayTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeStats",meta = (BindWidget))
	TObjectPtr<UTextBlock>  TextBlock_AttributeRatio;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeStats",meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeStats",meta = (BindWidget))
	TObjectPtr<UButton> Button_AddAttribute;
};
