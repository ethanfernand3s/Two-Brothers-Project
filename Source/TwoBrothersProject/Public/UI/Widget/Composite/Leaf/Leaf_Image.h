// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Items/TB_ItemFragments.h"
#include "UI/Widget/Composite/Leaf.h"
#include "Leaf_Image.generated.h"

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API ULeaf_Image : public ULeaf
{
	GENERATED_BODY()

public:
	
	void SetImage(UTexture2D* Texture) const;
	void SetBoxSize(const FVector2D& Size) const;
	void SetImageSize(const FVector2D& Size) const;
	FVector2D GetImageSize() const;
	void SetImageTint(const FLinearColor& ImageTint) const;

private:

	UPROPERTY(meta=(BindWidget))
	class UImage* Image;

	UPROPERTY(meta=(BindWidget))
	class USizeBox* SizeBox_Image;
};
