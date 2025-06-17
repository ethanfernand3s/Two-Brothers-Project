// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Characters/Data/BiomeDataAsset.h"
#include "Characters/Data/TribeData.h"
#include "UI/Widget/BaseUserWidget.h"
#include "AttributeMenuUserWidget.generated.h"

class UAttributeRowUserWidget;
class UAttributeXPBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UAttributeMenuUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()
public:

	virtual void SetWidgetController(UObject* InWidgetController) override;

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
	TObjectPtr<UAttributeXPBar> AttributeXPBar;

	// Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo|Stats",meta = (BindWidget))
	TObjectPtr<UAttributeRowUserWidget> AttributeRow_Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo|Stats",meta = (BindWidget))
	TObjectPtr<UAttributeRowUserWidget> AttributeRow_Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo|Stats",meta = (BindWidget))
	TObjectPtr<UAttributeRowUserWidget> AttributeRow_Oxygen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo|Stats",meta = (BindWidget))
	TObjectPtr<UAttributeRowUserWidget> AttributeRow_Hunger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo|Stats",meta = (BindWidget))
	TObjectPtr<UAttributeRowUserWidget> AttributeRow_Thirst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo|Stats",meta = (BindWidget))
	TObjectPtr<UAttributeRowUserWidget> AttributeRow_Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo|Stats",meta = (BindWidget))
	TObjectPtr<UAttributeRowUserWidget> AttributeRow_Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo|Stats",meta = (BindWidget))
	TObjectPtr<UAttributeRowUserWidget> AttributeRow_Speed;

	// TODO: Make this into subclass since it doesn't inherently have a button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo|Stats",meta = (BindWidget))
	TObjectPtr<UAttributeRowUserWidget> AttributeRow_Drowsiness;

protected:
	virtual void NativeConstruct() override;
	virtual void OnWidgetControllerSet() override;

private:
	void Single_SetAttributeRow(const FTBAttributeInfo& AttributeInfo);
	void CurrentAndMax_SetAttributeRow(const FTBAttributeInfo& CurrentAttributeInfo, const FTBAttributeInfo& MaxAttributeInfo);
	void SetType(const FTBAttributeInfo& TypeAttributeInfo);
	void SetupAttributeRowTags();

	void OnAttributePointsChanged(int NewAttributePoints);
	void OnBiomeChanged(const FBiomeInfo& NewBiomeInfo);
	void OnCharacterNameChanged(const FText& NewCharacterName);
	void OnLevelChanged(int NewLevel);
	void OnTribeDataChanged(const FTribeData& NewTribeData);
	void OnXPPercentChanged(float NewXPPercent);

	UPROPERTY()
	TMap<FGameplayTagContainer, UAttributeRowUserWidget*> MappedAttributeRows;
};