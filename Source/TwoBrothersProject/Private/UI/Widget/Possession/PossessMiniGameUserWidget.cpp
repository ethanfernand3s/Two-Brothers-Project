#include "UI/Widget/Possession/PossessMiniGameUserWidget.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Animation/WidgetAnimation.h"

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

	if (Bar)
	{
		Bar->SetFillColorAndOpacity(FLinearColor::Blue);
		Bar->SetPercent(Percent);
	}

	APlayerController* PC = GetOwningPlayer();
	if (PC && BeamEffectSystem)
	{
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			BeamEffectSystem, PC->GetPawn()->GetRootComponent(),
			NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			false, true, ENCPoolMethod::None, true);

		NiagaraComponent->SetVariableLinearColor(FName("User.PlayerColor"), PlayerColor);
		NiagaraComponent->SetVariableLinearColor(FName("User.EnemyColor"), EnemyColor);
		NiagaraComponent->SetVariableFloat(FName("User.Progress"), Percent);
	}

	SetKeyboardFocus();
}

void UPossessMiniGameUserWidget::NativeTick(const FGeometry& Geometry, float Dt)
{
	if (bDone) return;

	Elapsed += Dt;
	Percent = FMath::Clamp(Percent - EnemyPerSec * Dt, 0.f, 1.f);

	if (Bar)
		Bar->SetPercent(Percent);

	if (NiagaraComponent)
		NiagaraComponent->SetVariableFloat(FName("User.Progress"), Percent);

	if (Percent <= 0.f)
		Finish(false);
}

FReply UPossessMiniGameUserWidget::NativeOnKeyDown(const FGeometry& Geo, const FKeyEvent& KE)
{
	if (bDone) return Super::NativeOnKeyDown(Geo, KE);

	if (KE.GetKey() == EKeys::SpaceBar && !KE.IsRepeat())
	{
		Percent = FMath::Clamp(Percent + TapInc, 0.f, 1.f);
		if (Bar)
			Bar->SetPercent(Percent);

		if (NiagaraComponent)
			NiagaraComponent->SetVariableFloat(FName("User.Progress"), Percent);

		if (Percent >= 1.f)
			Finish(true);

		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(Geo, KE);
}

void UPossessMiniGameUserWidget::Finish(bool bWonIn)
{
	if (bDone) return;
	bDone = true;
	bWon = bWonIn;

	const float TimeUsed = Elapsed;
	PercentLeft = bWon ? (1.f - (Elapsed / NiceThreshold)) : 0.f;

	if (!bWon)
	{
		RemoveFromParent();
		return;
	}

	FString Result;
	if (TimeUsed < ExcellentThreshold)       Result = TEXT("Excellent!");
	else if (TimeUsed < GreatThreshold)      Result = TEXT("Great!");
	else if (TimeUsed < NiceThreshold)       Result = TEXT("Nice!");

	if (!Result.IsEmpty() && ResultText)
	{
		ResultText->SetText(FText::FromString(Result));
		PlayAnimation(ResultAnim);

		const float AnimLen = ResultAnim->GetEndTime();
		FTimerHandle Tmp;
		GetWorld()->GetTimerManager().SetTimer(Tmp, [this]() {
			RemoveFromParent();
		}, AnimLen, false);
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
