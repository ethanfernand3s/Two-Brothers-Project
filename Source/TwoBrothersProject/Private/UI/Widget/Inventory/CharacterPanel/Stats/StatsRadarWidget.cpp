#include "UI/Widget/Inventory/CharacterPanel/Stats/StatsRadarWidget.h"

#include "UI/Slate/StatsRadarSlate.h"

void UStatsRadarWidget::SetNormalizedStats(const TArray<float>& InStats)
{
	NormalizedStats = InStats;

	if (RadarSlateWidget.IsValid())
	{
		RadarSlateWidget->SetStats(NormalizedStats);
	}
}

void UStatsRadarWidget::UpdateStatNormalizedValue(int32 Index, float NewValue)
{
	if (RadarSlateWidget.IsValid())
	{
		RadarSlateWidget->SetStatAt(Index, NewValue);
	}
	else
	{
		// Ensure internal array grows safely even if Slate isn't valid yet
		if (!NormalizedStats.IsValidIndex(Index))
		{
			NormalizedStats.SetNumZeroed(FMath::Max(Index + 1, NormalizedStats.Num()));
		}
		NormalizedStats[Index] = FMath::Clamp(NewValue, 0.f, 1.f);
	}
}

void UStatsRadarWidget::SetOutlineColor(FLinearColor NewColor)
{
	OutlineColor = NewColor;

	if (RadarSlateWidget.IsValid())
	{
		RadarSlateWidget->SetOutlineColor(OutlineColor);
	}
}

void UStatsRadarWidget::SetFillColor(FLinearColor NewColor)
{
	FillColor = NewColor;

	if (RadarSlateWidget.IsValid())
	{
		RadarSlateWidget->SetColor(FillColor);
	}
}

TSharedRef<SWidget> UStatsRadarWidget::RebuildWidget()
{
	RadarSlateWidget = SNew(SStatsRadarSlate)
		.NormalizedStats(NormalizedStats)
		.FillColor(FillColor)
		.OutlineColor(OutlineColor);

	return RadarSlateWidget.ToSharedRef();
}

void UStatsRadarWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	RadarSlateWidget.Reset();
}