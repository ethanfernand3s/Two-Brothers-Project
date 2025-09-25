// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Helpers/Utils/WidgetUtils.h"

int32 UWidgetUtils::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
			// Rows + Column * TotalColumns
	return Position.X + ( Position.Y * Columns);
}