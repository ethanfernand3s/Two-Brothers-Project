// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystem/Data/CharacterCombatValues.h"
#include "Data/TribeData.h"
#include "Rarity/Data/Rarity.h"
#include "CharacterContextComponent.generated.h"


enum class EGrowthRate : uint8;
struct FBiomeInfo;
enum class ECharacterGender : uint8;
class UBiomeDataAsset;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInt32ValueChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTextValueChangedSignature, const FText&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTribeDataChangedSignature, const FTribeData&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBiomeDataChangedSignature, const FBiomeInfo&);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOBROTHERSPROJECT_API UCharacterContextComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UCharacterContextComponent();

	/** 
	 *  Used to initialize character stats outside the gameplay ability system.
	 *
	 * @note WeightedBST (Base Stat Total) should be calculated using the stats after IVs have been applied. Attributes
	 * should have high weights on combat related stats.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character Context")
	 void InitializeCharacterContext(
		const FText& InName,
		const int32 InLevel,
		const int32 InXP,
		const FTribeData& InTribeData,
		const ECharacterGender InGender
	);
	
	FORCEINLINE void InitializeRandomIVs() { IVSet.SetRandomValues(); }
	/**
	 * One time call for initializing BaseCombatPower | BaseXP | Rarity | GrowthRate
	 *
	 * @remark Currently randomly generating random base combat power if we make custom creatures change this to being
	 * passed instead of being generated.
	 * **/
	void InitializeCombatRelatedVars(float CustomBaseCombatPower = -1.f);
	
	// Setters
	FORCEINLINE void SetCharacterName(FText InName) { CharacterName = MoveTemp(InName); }
	FORCEINLINE void SetLevel(int32 NewLevel) { Level = NewLevel; }
	FORCEINLINE void SetXP(int32 NewXP) { XP = NewXP; }
	FORCEINLINE void SetTribeData(FTribeData InData) { TribeData = MoveTemp(InData); }
	FORCEINLINE void SetGender(ECharacterGender NewGender) { Gender = NewGender; }
	FORCEINLINE void SetBiomeData(UBiomeDataAsset* NewBiomeData) { BiomeData = NewBiomeData; }
	FORCEINLINE void SetAttributePoints(int32 NewPoints) { AttributePoints = NewPoints; }
	FORCEINLINE void SetRarity(ERarity NewRarity) { Rarity = NewRarity; }
	FORCEINLINE void SetGrowthRate(EGrowthRate NewGrowthRate) { LevelGrowthRate = NewGrowthRate; }
	FORCEINLINE void SetAuraColor(const FColor& NewAuraColor) { AuraColor = NewAuraColor; }
	
	// Getters
	FORCEINLINE FText GetCharacterName() const { return CharacterName; }
	FORCEINLINE int32 GetLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE const FTribeData& GetTribeData() const { return TribeData; }
	FORCEINLINE const ECharacterGender& GetGender() const { return Gender; }
	FORCEINLINE const UBiomeDataAsset* GetBiomeData() const { return BiomeData; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE ERarity GetRarity() const { return Rarity; }
	FORCEINLINE int32 GetBaseXP() const { return BaseXP; }
	FORCEINLINE int32 GetBaseCombatPower() const { return BaseCombatPower; }
	FORCEINLINE const FCharacterCombatValues& GetIVSet() const { return IVSet; }
	FORCEINLINE EGrowthRate GetGrowthRate() const {  return LevelGrowthRate; }
	FORCEINLINE FColor GetAuraColor() const { return AuraColor; }

	// Add To
	void AddToXP(int32 InXP);
	void AddToLevel(int32 InLevel);
	void AddToAttributePoints(int32 InPoints);

	// Delegates
	FOnTextValueChangedSignature OnCharacterNameChanged;
	FOnInt32ValueChangedSignature OnLevelChanged;
	FOnInt32ValueChangedSignature OnXPChanged;
	FOnTribeDataChangedSignature OnTribeDataChanged;
	FOnBiomeDataChangedSignature OnBiomeChanged;
	FOnInt32ValueChangedSignature OnAttributePointsChanged;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
private:

	// Mutable
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_CharacterName)
	FText CharacterName;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0.f;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_TribeData)
	FTribeData TribeData;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_BiomeData, meta = (AllowPrivateAccess))
	TObjectPtr<UBiomeDataAsset> BiomeData;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;

	// Immutable
	
	UPROPERTY(VisibleAnywhere, Replicated)
	int32 BaseXP = 0.f;
	
	UPROPERTY(VisibleAnywhere, Replicated)
	int32 BaseCombatPower = 0.f;

	UPROPERTY(VisibleAnywhere, Replicated)
	ECharacterGender Gender;

	UPROPERTY(VisibleAnywhere, Replicated)
	EGrowthRate LevelGrowthRate;
	
	UPROPERTY(VisibleAnywhere, Replicated)
	FCharacterCombatValues IVSet;

	UPROPERTY(VisibleAnywhere, Replicated)
	ERarity Rarity;

	UPROPERTY(VisibleAnywhere, Replicated)
	FColor AuraColor = FColor::Blue;
	
	// On Rep Functions
	
	UFUNCTION()
	void OnRep_CharacterName();
	
	UFUNCTION()
	void OnRep_Level();

	UFUNCTION()
	void OnRep_XP();

	UFUNCTION()
	void OnRep_TribeData();

	UFUNCTION()
	void OnRep_BiomeData();

	UFUNCTION()
	void OnRep_AttributePoints();
};