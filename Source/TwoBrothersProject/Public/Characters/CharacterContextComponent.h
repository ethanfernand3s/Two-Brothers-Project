// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/TribeData.h"
#include "CharacterContextComponent.generated.h"


enum class ECharacterGender : uint8;
class UBiomeDataAsset;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInt32ValueChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTextValueChangedSignature, const FText&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTribeDataChangedSignature, const FTribeData&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBiomeDataChangedSignature, const UBiomeDataAsset*);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOBROTHERSPROJECT_API UCharacterContextComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UCharacterContextComponent();

	UFUNCTION(BlueprintCallable, Category = "Character Context")
	void InitializeCharacterContext(
		const FText& InName,
		int32 InLevel,
		int32 InXP,
		const FTribeData& InTribeData,
		ECharacterGender InGender,
		UBiomeDataAsset* InBiomeData,
		int32 InAttributePoints
	);
	
	// Setters
	FORCEINLINE void SetCharacterName(FText InName) { CharacterName = MoveTemp(InName); }
	FORCEINLINE void SetLevel(int32 NewLevel) { Level = NewLevel; }
	FORCEINLINE void SetXP(int32 NewXP) { XP = NewXP; }
	FORCEINLINE void SetTribeData(FTribeData InData) { TribeData = MoveTemp(InData); }
	FORCEINLINE void SetGender(ECharacterGender NewGender) { Gender = NewGender; }
	FORCEINLINE void SetBiomeData(UBiomeDataAsset* NewBiomeData) { BiomeData = NewBiomeData; }
	FORCEINLINE void SetAttributePoints(int32 NewPoints) { AttributePoints = NewPoints; }
	
	// Getters
	FORCEINLINE FText GetCharacterName() const { return CharacterName; }
	FORCEINLINE int32 GetLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE const FTribeData& GetTribeData() const { return TribeData; }
	FORCEINLINE const ECharacterGender& GetGender() const { return Gender; }
	FORCEINLINE const UBiomeDataAsset* GetBiomeData() const { return BiomeData; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }


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

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_CharacterName)
	FText CharacterName;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0.f;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_TribeData)
	FTribeData TribeData;

	UPROPERTY(VisibleAnywhere, Replicated)
	ECharacterGender Gender;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_BiomeData)
	TObjectPtr<UBiomeDataAsset> BiomeData;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;

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