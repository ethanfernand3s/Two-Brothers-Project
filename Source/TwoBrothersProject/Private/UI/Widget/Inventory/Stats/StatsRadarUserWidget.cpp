#include "UI/Widget/Inventory/Stats/StatsRadarUserWidget.h"
#include "UI/Widget/Inventory/Stats/StatsRadarWidget.h"
#include "Components/PanelWidget.h"

void UStatsRadarUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (RadarWidget)
	{
		RadarWidget->SetFillColor(FillColor);
		RadarWidget->SetOutlineColor(OutlineColor);
		
		if (Container && !Container->HasChild(RadarWidget))
		{
			Container->AddChild(RadarWidget);
		}
	}
}

void UStatsRadarUserWidget::UpdateSingleRadarStat(int32 Index, float Value)
{
	if (RadarWidget)
	{
		RadarWidget->UpdateStatNormalizedValue(Index, Value);
	}
}
