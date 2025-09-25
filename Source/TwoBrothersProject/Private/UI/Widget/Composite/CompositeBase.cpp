// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Composite/CompositeBase.h"

void UCompositeBase::Collapse()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UCompositeBase::Expand()
{
	SetVisibility(ESlateVisibility::Visible);
}
