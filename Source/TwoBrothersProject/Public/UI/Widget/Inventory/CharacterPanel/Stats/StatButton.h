// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/Button.h"
#include "UI/Widget/BaseUserWidget.h"
#include "StatButton.generated.h"

class UStatButton;

class UTextBlock;
class UButton;

DECLARE_DELEGATE_OneParam(FOnStatButtonClicked, const UStatButton* /* StatButton */);
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UStatButton : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	
	void UpdateButtonStyle();
	void SetStatText(const FText& NewStatText) const;
	virtual void SetIsEnabled(bool bInIsEnabled) override;
	
	FOnStatButtonClicked OnStatButtonClicked;

protected:
	virtual void NativeOnInitialized() override;
	UFUNCTION()
	void StatButtonClicked();
	
private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Stat;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Stat;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category = "Style")
	TObjectPtr<UTexture2D> NormalImage;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category = "Style")
	TObjectPtr<UTexture2D> DisabledImage;
	
	/* Set Externally */
	FGameplayTag GameplayTag;
	
protected:
	
	virtual void NativePreConstruct() override;
};
