
#include "UI/Widget/Inventory/CharacterPanel/Stats/StatsRadarUserWidget.h"

#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "UI/Widget/Inventory/CharacterPanel/Stats/StatsRadarWidget.h"

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

void UStatsRadarUserWidget::SetCharacterIcon(UTexture2D* NewCharacterIcon)
{
	Image_CharacterIcon->SetBrushFromTexture(NewCharacterIcon);
}
