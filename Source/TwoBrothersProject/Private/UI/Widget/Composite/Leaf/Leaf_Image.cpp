// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Composite/Leaf/Leaf_Image.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"

void ULeaf_Image::SetImage(UTexture2D* Texture) const
{
	Image->SetBrushFromTexture(Texture);
}

void ULeaf_Image::SetBoxSize(const FVector2D& Size) const
{
	SizeBox_Image->SetWidthOverride(Size.X);
	SizeBox_Image->SetHeightOverride(Size.Y);
}

void ULeaf_Image::SetImageSize(const FVector2D& Size) const
{
	Image->SetDesiredSizeOverride(Size);
}

FVector2D ULeaf_Image::GetImageSize() const
{
	return Image->GetDesiredSize();
}

void ULeaf_Image::SetImageTint(const FLinearColor& ImageTint) const
{
	Image->SetBrushTintColor(ImageTint);
}
