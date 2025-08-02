#include "UI/Slate/StatsRadarSlate.h"

int32 SStatsRadarSlate::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FVector2D Size = AllottedGeometry.GetLocalSize();
    const FVector2D Center = Size * 0.5f;
    const float Radius = FMath::Min(Size.X, Size.Y) * 0.45f;

    constexpr int32 NumStats = 6;
    static constexpr float Angles[NumStats] = { -90.f, -30.f, 30.f, 90.f, 150.f, 210.f };

    TArray<FVector2D> Points;
    Points.Reserve(NumStats);

    // Compute radar vertices
    for (int32 i = 0; i < NumStats; ++i)
    {
        float Value = NormalizedStats.IsValidIndex(i) ? FMath::Clamp(NormalizedStats[i], 0.f, 1.f) : 0.f;
        float Radians = FMath::DegreesToRadians(Angles[i]);
        FVector2D Pos = Center + FVector2D(FMath::Cos(Radians), FMath::Sin(Radians)) * (Value * Radius);
        Points.Add(Pos);
    }

    // Build triangle-fan buffers
    TArray<FSlateVertex> VertexBuffer;
    TArray<SlateIndex> IndexBuffer;
    VertexBuffer.Reserve(NumStats + 1);
    IndexBuffer.Reserve(NumStats * 3);

    const FColor QuadColor = FillColor.ToFColor(true);
    const FSlateRenderTransform& RenderTransform = AllottedGeometry.GetAccumulatedRenderTransform();

    // Center vertex
    VertexBuffer.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(
        RenderTransform,
        FVector2f(Center),
        FVector4f(0,0,0,0),
        FVector2f(0,0),
        QuadColor,
        FColor()
    ));

    // Outer vertices
    for (const FVector2D& P : Points)
    {
        VertexBuffer.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(
            RenderTransform,
            FVector2f(P),
            FVector4f(0,0,0,0),
            FVector2f(0,0),
            QuadColor,
            FColor()
        ));
    }

    // Build fan indices
    for (int32 i = 1; i <= NumStats; ++i)
    {
        IndexBuffer.Add(0);
        IndexBuffer.Add(i);
        IndexBuffer.Add(i % NumStats + 1);
    }

    // Get brush handle for color fill
    const FSlateBrush* WhiteBrush = FCoreStyle::Get().GetBrush("WhiteBrush");
    auto& BrushHandle = WhiteBrush->GetRenderingResource();

    // Draw filled polygon
    FSlateDrawElement::MakeCustomVerts(
        OutDrawElements,
        LayerId,
        BrushHandle,
        VertexBuffer,
        IndexBuffer,
        nullptr,
        0,
        0,
        ESlateDrawEffect::None
    );

    // Draw polygon outline
    TArray<FVector2D> LoopPoints = Points;
    LoopPoints.Add(Points[0]); // Close the loop

    FSlateDrawElement::MakeLines(
        OutDrawElements,
        LayerId + 1,
        AllottedGeometry.ToPaintGeometry(),
        LoopPoints,
        ESlateDrawEffect::None,
        OutlineColor,
        true,
        1.5f
    );

    // Call base class to handle child visuals
    return SCompoundWidget::OnPaint(
        Args,
        AllottedGeometry,
        MyCullingRect,
        OutDrawElements,
        LayerId + 2,
        InWidgetStyle,
        bParentEnabled
    );
}

void SStatsRadarSlate::SetStatAt(int32 Index, float NewValue)
{
    if (!NormalizedStats.IsValidIndex(Index))
    {
        NormalizedStats.SetNumZeroed(FMath::Max(Index + 1, NormalizedStats.Num()));
    }

    NormalizedStats[Index] = FMath::Clamp(NewValue, 0.f, 1.f);

    // This will trigger a redraw
    Invalidate(EInvalidateWidgetReason::Paint);
}