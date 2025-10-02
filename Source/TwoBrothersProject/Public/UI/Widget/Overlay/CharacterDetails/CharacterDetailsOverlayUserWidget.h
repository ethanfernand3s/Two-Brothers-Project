// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "CharacterDetailsOverlayUserWidget.generated.h"

class UImage;
class UTextBlock;
class USlotContainerUserWidget;
class UProgressBar;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UCharacterDetailsOverlayUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	
	// Character Context Callbacks
	void OnAttributePointsUpdated(int32 NewAttributePoints);
	void OnCharacterIconChanged(UTexture2D* CharacterIcon);
	void OnLevelUpdated(int32 NewLevel);
	void OnXPPercentageUpdated(float NewXPPercentage);

	// Attribute Callbacks
	void OnHealthUpdated(float NewHealth);
	void OnMaxHealthUpdated(float NewMaxHealth);
	void OnAuraUpdated(float NewAura);
	void OnMaxAuraUpdated(float NewMaxAura);
	
private:
	
#pragma region ProgressBars
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Health;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Aura;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_XP;
#pragma endregion ProgressBars

#pragma region TextBlock
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Level;

#pragma endregion TextBlock

#pragma region Image
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_IconFill;

#pragma endregion Image
	
	bool bIsAttributePointsEmpty = true;
	
	float Health = 100;
	float MaxHealth = 100;
	float Aura = 100;
	float MaxAura = 100;
};
