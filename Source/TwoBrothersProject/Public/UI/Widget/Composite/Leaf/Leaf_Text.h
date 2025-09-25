// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/Composite/Leaf.h"
#include "Leaf_Text.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API ULeaf_Text : public ULeaf
{
	GENERATED_BODY()

public:
	
	void SetText(const FText& Text) const;
	void SetTextColor(const FLinearColor& TextColor) const;

protected:
	
	virtual void NativePreConstruct() override;

private:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_LeafText;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	int32 FontSize{12};
};
