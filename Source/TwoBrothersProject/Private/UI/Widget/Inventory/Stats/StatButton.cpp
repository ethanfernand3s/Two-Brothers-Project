#include "UI/Widget/Inventory/Stats/StatButton.h"

#include "Components/Button.h"

void UStatButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateButtonStyle(); // Apply the images at design time & runtime
}

void UStatButton::UpdateButtonStyle()
{
	if (!Button_Stat) return;

	auto MakeBrush = [](UTexture2D* Texture) -> FSlateBrush
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(Texture);
		if (Texture)
		{
			Brush.ImageSize = FVector2D(Texture->GetSizeX(), Texture->GetSizeY());
		}
		Brush.DrawAs = ESlateBrushDrawType::Image;
		return Brush;
	};

	FButtonStyle NewStyle;

	if (NormalImage)
	{
		FSlateBrush NormalBrush = MakeBrush(NormalImage);
		NormalBrush.TintColor = FSlateColor(FLinearColor(.85f, .85f, .85f, 1.0f));  // Base color
		NewStyle.SetNormal(NormalBrush);

		// Use same brush as base for hover & press, but change tint
		FSlateBrush HoveredBrush = NormalBrush;
		HoveredBrush.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.0f)); 
		NewStyle.SetHovered(HoveredBrush);

		FSlateBrush PressedBrush = NormalBrush;
		PressedBrush.TintColor = FSlateColor(FLinearColor(0.7f, 0.7f, 0.7f, 1.0f));
		NewStyle.SetPressed(PressedBrush);
	}

	if (DisabledImage)
	{
		FSlateBrush DisabledBrush = MakeBrush(DisabledImage);
		DisabledBrush.TintColor = FSlateColor(FLinearColor::White); // No tint
		NewStyle.SetDisabled(DisabledBrush);
	}

	Button_Stat->SetStyle(NewStyle);
}