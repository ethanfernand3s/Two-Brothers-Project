// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.h"
#include "AbilitySystem/Data/CreatureType.h"
#include "Characters/Data/TribeData.h"
#include "InventoryWidgetController.generated.h"

class UAbilityCardDataAsset;
class UGenderDataAsset;
class UCreatureTypeDataAsset;
enum class ECharacterGender : uint8;
struct FGameplayTag;
struct FTBAttributeInfo;

// Attribute Delegates
// For broadcasting both current and max values (e.g., Health / MaxHealth)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributeCurrentAndMaxChangedSignature, const FTBAttributeInfo&, const FTBAttributeInfo&);
// For broadcasting a single value (e.g., Strength, Speed)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChangedSignature, const FTBAttributeInfo&);

// Character Context Delegates
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttributePointsChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTextChangedSignature, const FText&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGenderSetSignature, const ECharacterGender&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAuraColorSetSignature, const FColor&);
using FCreatureTypePair = TPair<ECreatureType, ECreatureType>;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreatureTypesSetSignature, const FCreatureTypePair&);


UCLASS()
class TWOBROTHERSPROJECT_API UInventoryWidgetController : public UBaseWidgetController
{

	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	
	// Attribute Set Delegates
	FOnAttributeCurrentAndMaxChangedSignature CurrentAndMax_AttributeInfoDelegate;
	FOnAttributeValueChangedSignature Single_AttributeInfoDelegate;

	// Character Context Delegates
	FOnAttributePointsChangedSignature OnAttributePointsChangedDelegate;
	FOnLevelChangedSignature OnLevelChangedDelegate;
	FOnTextChangedSignature OnCharacterNameChangedDelegate;
	FOnTextChangedSignature OnTribeNameChangedDelegate;
	FOnGenderSetSignature OnGenderSetDelegate;
	FOnAuraColorSetSignature OnAuraColorSetDelegate;
	FOnCreatureTypesSetSignature OnCreatureTypesSetDelegate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataAssets")
	TObjectPtr<UCreatureTypeDataAsset> CreatureTypeData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataAssets")
	TObjectPtr<UGenderDataAsset> GenderData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataAssets")
	TObjectPtr<UAbilityCardDataAsset> AbilityCardData;

private:

	void OnTribeDataChanged(const FTribeData& TribeData);
	void OnAttributePointsChanged(int NewAttributePoints);
	void OnCharacterNameChanged(const FText& NewCharacterName);
	void OnLevelChanged(int NewLevel);
};