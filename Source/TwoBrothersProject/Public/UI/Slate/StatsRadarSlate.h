#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SStatsRadarSlate : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SStatsRadarSlate) {}
	SLATE_ARGUMENT(TArray<float>, NormalizedStats)
	SLATE_ARGUMENT(FLinearColor, FillColor)
    SLATE_ARGUMENT(FLinearColor, OutlineColor)
SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		NormalizedStats = InArgs._NormalizedStats;
		FillColor = InArgs._FillColor;
	}

	void SetStats(const TArray<float>& Stats)
	{
		NormalizedStats = Stats;
		Invalidate(EInvalidateWidget::Paint);
	}

	void SetColor(const FLinearColor& Color)
	{
		FillColor = Color;
		Invalidate(EInvalidateWidget::Paint);
	}

	void SetOutlineColor(const FLinearColor& Color) 
	{
		OutlineColor = Color;
		Invalidate(EInvalidateWidget::Paint);
	}
	
	void SetStatAt(int32 Index, float NewValue);

protected:
	TArray<float> NormalizedStats;
	FLinearColor FillColor = FLinearColor(0.1f, 0.7f, 1.0f, 0.4f);;
	FLinearColor OutlineColor = FLinearColor::White;
	
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
						  const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
						  int32 LayerId, const FWidgetStyle& InWidgetStyle,
						  bool bParentEnabled) const override;
};
