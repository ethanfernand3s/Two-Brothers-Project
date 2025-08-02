#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "StatsRadarUserWidget.generated.h"

class UImage;
class UStatsRadarWidget;
class UPanelWidget;

/**
 * UMG wrapper for the Slate-based radar stat display.
 */
UCLASS()
class TWOBROTHERSPROJECT_API UStatsRadarUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	/** Update a specific stat's normalized radar value. */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateSingleRadarStat(int32 Index, float Value);

protected:
	/** Used to apply design-time preview logic */
	virtual void NativePreConstruct() override;

	/** Low-level radar widget that draws the radar shape */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatsRadarWidget> RadarWidget;

	/** Panel container from UMG (e.g., CanvasPanel or SizeBox) */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> Container;

	/** Image of current  */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_CurrentCharacterIcon;

public:
	/** Fill color of the radar stat polygon (can be changed in the Details panel) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FLinearColor FillColor = FLinearColor(0.1f, 0.7f, 1.0f, 0.4f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FLinearColor OutlineColor = FLinearColor::White;
};
