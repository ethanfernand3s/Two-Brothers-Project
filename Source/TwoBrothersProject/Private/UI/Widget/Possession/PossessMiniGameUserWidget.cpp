#include "UI/Widget/Possession/PossessMiniGameUserWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Animation/WidgetAnimation.h"
#include "NiagaraComponent.h"
#include "NiagaraUIComponent.h"

void UPossessMiniGameUserWidget::Init(float InStartingChance, float InTapInc, float InEnemyPerSec,
                                      const FColor& InPlayerColor, const FColor& InEnemyColor)
{
	Percent     = FMath::Clamp(InStartingChance, 0.f, 1.f);
	TapInc      = FMath::Clamp(InTapInc, 0.f, 1.f);
	EnemyPerSec = FMath::Clamp(InEnemyPerSec, 0.f, 1.f);
	Elapsed     = 0.f;
	bDone       = false;

	PlayerColor = InPlayerColor;
	EnemyColor  = InEnemyColor;
}

void UPossessMiniGameUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (NSWidgetPossessProgress)
	{
		if (UNiagaraComponent* NiagaraComp = NSWidgetPossessProgress->GetNiagaraComponent())
		{
			NiagaraComp->SetVariableLinearColor(TEXT("User.PlayerAuraColor"), PlayerColor);
			NiagaraComp->SetVariableLinearColor(TEXT("User.EnemyAuraColor"), EnemyColor);

			const float XOffset = (Percent - 0.5f) * 2.f * 935.f;
			NiagaraComp->SetVariableVec3(TEXT("User.PossessProgress"), FVector(XOffset, 0.f, 0.f));
		}
	}

	SetKeyboardFocus();
}

void UPossessMiniGameUserWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	if (bDone) return;

	Elapsed += DeltaTime;
	Percent = FMath::Clamp(Percent - EnemyPerSec * DeltaTime, 0.f, 1.f);
	OnGaugeChanged.ExecuteIfBound(Percent);
	
	if (NSWidgetPossessProgress)
	{
		if (UNiagaraComponent* NiagaraComp = NSWidgetPossessProgress->GetNiagaraComponent())
		{
			const float XOffset = (Percent - 0.5f) * 2.f * 935.f;
			NiagaraComp->SetVariableVec3(TEXT("User.PossessProgress"), FVector(XOffset, 0.f, 0.f));
		}
	}

	if (Percent <= 0.f)
		Finish(false);
}

FReply UPossessMiniGameUserWidget::NativeOnKeyDown(const FGeometry& Geo, const FKeyEvent& KeyEvent)
{
	if (bDone) return Super::NativeOnKeyDown(Geo, KeyEvent);

	if (KeyEvent.GetKey() == EKeys::SpaceBar && !KeyEvent.IsRepeat())
	{
		Percent = FMath::Clamp(Percent + TapInc, 0.f, 1.f);
		if (NSWidgetPossessProgress)
		{
			if (UNiagaraComponent* NiagaraComp = NSWidgetPossessProgress->GetNiagaraComponent())
			{
				const float XOffset = (Percent - 0.5f) * 2.f * 935.f;
				NiagaraComp->SetVariableVec3(TEXT("User.PossessProgress"), FVector(XOffset, 0.f, 0.f));
				OnSpaceBarPressed.ExecuteIfBound();
			}
		}
		if (Percent >= 1.f)
			Finish(true);

		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(Geo, KeyEvent);
}

void UPossessMiniGameUserWidget::Finish(bool bWonIn)
{
	if (bDone) return;
	bDone = true;
	bWon = bWonIn;

	NSWidgetPossessProgress->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_ResultText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	const float TimeUsed = Elapsed;
	PercentLeft = bWon ? (1.f - (Elapsed / NiceThreshold)) : 0.f;

	if (!bWon)
	{
		RemoveFromParent();
		return;
	}

	FString Result;
	if (TimeUsed < ExcellentThreshold)
		Result = TEXT("Excellent!");
	else if (TimeUsed < GreatThreshold)
		Result = TEXT("Great!");
	else if (TimeUsed < NiceThreshold)
		Result = TEXT("Nice!");

	if (!Result.IsEmpty() && TextBlock_ResultText)
	{
		TextBlock_ResultText->SetText(FText::FromString(Result));
		PlayAnimation(ResultAnim);

		const float AnimLength = ResultAnim->GetEndTime();
		FTimerHandle CleanupTimer;
		GetWorld()->GetTimerManager().SetTimer(CleanupTimer, [this]()
		{
			RemoveFromParent();
		}, AnimLength, false);
	}
	else
	{
		RemoveFromParent();
	}
}

void UPossessMiniGameUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
	OnFinished.Broadcast(bWon, PercentLeft);
}
