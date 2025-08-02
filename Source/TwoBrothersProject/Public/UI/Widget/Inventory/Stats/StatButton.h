// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/Widget/BaseUserWidget.h"
#include "StatButton.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UStatButton : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (BindWidget))
	TObjectPtr<UButton> Button_Stat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Stat;

	FGameplayTag GameplayTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	TObjectPtr<UTexture2D> NormalImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	TObjectPtr<UTexture2D> DisabledImage;

	UFUNCTION(BlueprintCallable, Category = "Style")
	void UpdateButtonStyle();

protected:
	virtual void NativePreConstruct() override;
};
