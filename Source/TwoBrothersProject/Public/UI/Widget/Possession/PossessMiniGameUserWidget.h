#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystemWidget.h"
#include "Blueprint/UserWidget.h"
#include "PossessMiniGameUserWidget.generated.h"

class UNiagaraSystemWidget;
class UProgressBar;
class UTextBlock;
class UWidgetAnimation;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMiniGameFinishedDelegate, bool, bWon, float, NormalizedTimeLeft);
DECLARE_DELEGATE_OneParam(FGaugeChanged, float);
DECLARE_DELEGATE(FSpaceBarPressed);

UCLASS()
class TWOBROTHERSPROJECT_API UPossessMiniGameUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(float InStartingChance, float InTapInc, float InEnemyPerSec,
			  const FColor& InPlayerColor, const FColor& InEnemyColor);

	UPROPERTY(BlueprintAssignable)
	FMiniGameFinishedDelegate OnFinished;
	
	FGaugeChanged OnGaugeChanged;
	FSpaceBarPressed OnSpaceBarPressed;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& Geometry, const FKeyEvent& KeyEvent) override;
	virtual void NativeDestruct() override;

private:
	void Finish(bool bWon);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_ResultText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ResultAnim;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNiagaraSystemWidget> NSWidgetPossessProgress;

	FLinearColor PlayerColor;
	FLinearColor EnemyColor;

	float Percent = 0.f;
	float TapInc = 0.f;
	float EnemyPerSec = 0.f;
	float Elapsed = 0.f;

	bool bDone = false;
	bool bWon = false;
	float PercentLeft = 0.f;

	const float ExcellentThreshold = 2.f;
	const float GreatThreshold     = 4.f;
	const float NiceThreshold      = 6.f;
};
