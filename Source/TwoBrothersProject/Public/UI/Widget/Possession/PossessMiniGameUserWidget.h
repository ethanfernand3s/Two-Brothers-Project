// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "PossessMiniGameUserWidget.generated.h"

class UTextBlock;
class UProgressBar;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FMiniGameFinishedSig, bool, bBarWon, float, PercentTimeLeft);

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UPossessMiniGameUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(float InStartingChance,
		      float InDuration = 10.f,
			  float InTapInc   = 0.06f,
			  float InEnemyPerSec = 0.05f);

	UPROPERTY(BlueprintAssignable, Category="MiniGame")
	FMiniGameFinishedSig OnFinished;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geo, float Dt) override;
	virtual FReply NativeOnKeyDown(const FGeometry& Geo,
								   const FKeyEvent& KE) override;
	virtual void NativeDestruct() override;

private:
	void Finish(bool bWonIn);

	UPROPERTY(meta=(BindWidget)) UProgressBar* Bar = nullptr;
	UPROPERTY(meta=(BindWidget)) UTextBlock* Textblock_PossessChance = nullptr;
	UPROPERTY(Transient, meta=(BindWidgetAnim)) UWidgetAnimation* CalculatePossessionChance = nullptr;

	float Dur          = 10.f;     // seconds
	float TapInc       = 0.06f;    // percent / space
	float EnemyPerSec  = 0.05f;    // percent / sec
	float Elapsed      = 0.f;
	float Percent      = 0.5f;
	bool  bDone        = false;

	float PercentLeft = 0.f;
	bool bWon = false;
	
	float StartingChance = 0.f;
};
