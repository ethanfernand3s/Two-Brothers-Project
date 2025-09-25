// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TribeData.generated.h"

USTRUCT(BlueprintType)
struct FTribeData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FText TribeName;

	UPROPERTY(EditAnywhere)
	FText TribeDescription;

	UPROPERTY(EditAnywhere)
	UTexture2D* TribeIcon = nullptr;

	UPROPERTY(EditAnywhere)
	FLinearColor TribeColor = FLinearColor::White;

	FTribeData() = default;

	FTribeData(const FText& InName, const FText& InDescription, UTexture2D* InIcon = nullptr, const FLinearColor& InColor = FLinearColor::White)
		: TribeName(InName)
		, TribeDescription(InDescription)
		, TribeIcon(InIcon)
		, TribeColor(InColor)
	{}
};