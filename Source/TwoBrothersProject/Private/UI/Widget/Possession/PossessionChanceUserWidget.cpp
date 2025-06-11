// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Possession/PossessionChanceUserWidget.h"

#include "Components/TextBlock.h"

void UPossessionChanceUserWidget::SetPossessionChance(float InChance)
{
	PossessionChance->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), InChance)));
}
