#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "StatsRadarWidget.generated.h"

class SStatsRadarSlate;

UCLASS()
class TWOBROTHERSPROJECT_API UStatsRadarWidget : public UWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetNormalizedStats(const TArray<float>& InStats);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateStatNormalizedValue(int32 Index, float NewValue);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetOutlineColor(FLinearColor NewColor);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetFillColor(FLinearColor NewColor);

protected:
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:

	UPROPERTY()
	TArray<float> NormalizedStats;
	
	TSharedPtr<SStatsRadarSlate> RadarSlateWidget;

	/* Externally Set */
	FLinearColor FillColor = FLinearColor(0.1f, 0.7f, 1.0f, 0.4f);
	FLinearColor OutlineColor = FLinearColor::White;
};
