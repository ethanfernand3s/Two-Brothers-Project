// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "SlotUserWidget.generated.h"

class UAbilityCardUserWidget;
class USizeBox;
class UButton;
enum class EAbilityType : uint8;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API USlotUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	void FillSlot(UAbilityCardUserWidget* NewCard);
	void EmptySlot();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	EAbilityType AbilityPreference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> Button_Slot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_AbilityCardHolder;
};
