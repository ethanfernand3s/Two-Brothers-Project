// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Possession/PossessMiniGameUserWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/ProgressBar.h"
#include "Player/TBPlayerController.h"

void UPossessMiniGameUserWidget::Init(float InStartingChance, float InDur, float InTapInc, float InEnemyPerSec)
{
	Dur         = FMath::Max(0.1f, InDur);
	TapInc      = FMath::Max(0.f , InTapInc);
	EnemyPerSec = FMath::Max(0.f , InEnemyPerSec);

	StartingChance = InStartingChance;
}

void UPossessMiniGameUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Bar)
	{
		Bar->SetFillColorAndOpacity(FLinearColor::Blue);
		Bar->SetPercent(Percent);
	}

	SetKeyboardFocus();
}

void UPossessMiniGameUserWidget::NativeTick(const FGeometry& Geometry, float Dt)
{
	if (bDone) return;

	Elapsed += Dt;
	Percent -= EnemyPerSec * Dt;
	Percent  = FMath::Clamp(Percent, 0.f, 1.f);

	if (Bar) Bar->SetPercent(Percent);

	if (Percent <= 0.f || Elapsed >= Dur)
		Finish(false);
}

FReply UPossessMiniGameUserWidget::NativeOnKeyDown(const FGeometry& Geo, const FKeyEvent& KE)
{
	if (bDone) return Super::NativeOnKeyDown( Geo ,KE);

	if (KE.GetKey() == EKeys::SpaceBar)
	{
		Percent = FMath::Clamp(Percent + TapInc, 0.f, 1.f);
		if (Bar) Bar->SetPercent(Percent);
		if (Percent >= 1.f)
			Finish(true);

		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(Geo ,KE);
}

void UPossessMiniGameUserWidget::Finish(bool bWonIn)
{
	if (bDone) return;
	bDone = true;

	const float TimeLeft    = FMath::Clamp(Dur - Elapsed, 0.f, Dur);
	PercentLeft = bWon ? TimeLeft / Dur : 0.f;
	bWon = bWonIn;

	// Play additive-score animation if it exists
	if (CalculatePossessionChance)
	{
		PlayAnimation(CalculatePossessionChance);

		// remove widget when the animation finishes
		const float AnimLen = CalculatePossessionChance->GetEndTime();      // duration in seconds
		FTimerHandle Tmp;
		GetWorld()->GetTimerManager().SetTimer(
			Tmp,
			[this]()
			{
				RemoveFromParent();
			},
			AnimLen,
			false);
	}
	else
	{
		RemoveFromParent();     // no animation; close immediately
	}
}

void UPossessMiniGameUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
	OnFinished.Broadcast(bWon, PercentLeft);
}
