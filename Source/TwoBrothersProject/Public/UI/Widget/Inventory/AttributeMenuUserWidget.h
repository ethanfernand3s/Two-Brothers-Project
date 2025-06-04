// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "AttributeMenuUserWidget.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UAttributeMenuUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Tribe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Biome;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo",meta = (BindWidget))
	TObjectPtr<UTextBlock>  TextBlock_PointsAvailable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeStats",meta = (BindWidget))
	TObjectPtr<UTextBlock>  TextBlock_TextBlock_XpAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeStats",meta = (BindWidget))
	TObjectPtr<UTextBlock>  TextBlock_TextBlock_MaxXpAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeStats",meta = (BindWidget))
	TObjectPtr<UProgressBar> UProgressBar_XpProgressAmount;
	



};
