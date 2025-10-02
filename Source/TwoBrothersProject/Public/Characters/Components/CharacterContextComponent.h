// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "AbilitySystem/Data/CharacterCombatValues.h"
#include "../Data/TribeData.h"
#include "CharacterContextComponent.generated.h"


enum class EGrowthRate : uint8;
class UBiomeDataAsset;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInt32ValueChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTextValueChangedSignature, const FText&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTextureChangedSignature, UTexture2D*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTribeDataChangedSignature, const FTribeData&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTagContainerChangedSignature, const FGameplayTagContainer&);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOBROTHERSPROJECT_API UCharacterContextComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UCharacterContextComponent();
	
	/* Used to initialize character stats outside the gameplay ability system. */
	UFUNCTION(BlueprintCallable, Category = "Character Context")
	 void InitializeCharacterContext(float ExternalBaseCombatPower = -1);

	// TODO: Move random gen logic to creature factory spawner 
	/**
	 * One time call for initializing BaseCombatPower | BaseXP | Rarity | GrowthRate
	 * @remark All items, body parts, creatures should have a base power or emc.
	 * Effectiveness of items (including body parts) should be influenced by spirit of creature,
	 * the one part that belongs only to that creature.
	 * @attention !!! TEMPORARY - NEED TO MOVE TO CREATURE FACTORY SPAWNER !!!
	 **/
	void InitializeCombatRelatedVars(float ExternalBaseCombatPower = -1);
	
	/* TODO: Implement load character context & save character context */
	
	// Setters
	FORCEINLINE void SetCharacterName(FText InName)
	{
		CharacterName = MoveTemp(InName);
		OnCharacterNameChanged.Broadcast(InName);
	}
	FORCEINLINE void SetCharacterIcon(UTexture2D* InCharacterIcon)
	{
		CharacterIcon = MoveTemp(InCharacterIcon);
		OnCharacterIconChanged.Broadcast(CharacterIcon);
	}
	FORCEINLINE void SetLevel(int32 NewLevel)
	{
		Level = NewLevel;
		OnLevelChanged.Broadcast(NewLevel);
	}
	FORCEINLINE void SetXP(int32 NewXP)
	{
		XP = NewXP;
		OnXPChanged.Broadcast(NewXP);
	}
	FORCEINLINE void SetTribeData(FTribeData InData)
	{
		TribeData = MoveTemp(InData);
		OnTribeDataChanged.Broadcast(InData);
	}
	FORCEINLINE void SetAttributePoints(int32 NewPoints)
	{
		AttributePoints = NewPoints;
		OnAttributePointsChanged.Broadcast(NewPoints);
	}
	FORCEINLINE void SetGender(const FGameplayTag&  NewGender) { Gender = NewGender; }
	FORCEINLINE void SetRarity(const FGameplayTag& NewRarity) { Rarity = NewRarity; }
	FORCEINLINE void SetGrowthRate(EGrowthRate NewGrowthRate) { LevelGrowthRate = NewGrowthRate; }
	FORCEINLINE void SetCreatureTypes(const FGameplayTagContainer& NewCreatureTypes) { CreatureTypeTags = NewCreatureTypes; }
	FORCEINLINE void SetBodySockets(const FGameplayTagContainer& NewBodyPartTags) { BodyPartTags = NewBodyPartTags; }
	
	// Getters
	FORCEINLINE FText GetCharacterName() const { return CharacterName; }
	FORCEINLINE UTexture2D* GetCharacterIcon() const { return CharacterIcon; }
	FORCEINLINE int32 GetLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE const FTribeData& GetTribeData() const { return TribeData; }
	FORCEINLINE const FGameplayTag& GetGender() const { return Gender; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE const FGameplayTag& GetRarity() const { return Rarity; }
	FORCEINLINE int32 GetBaseXP() const { return BaseXP; }
	FORCEINLINE int32 GetBaseCombatPower() const { return BaseCombatPower; }
	FORCEINLINE const FCharacterCombatValues& GetIVSet() const { return IVSet; }
	FORCEINLINE EGrowthRate GetGrowthRate() const {  return LevelGrowthRate; }
	FORCEINLINE const FGameplayTagContainer& GetCreatureTypes() const { return CreatureTypeTags; }
	FORCEINLINE const FGameplayTagContainer& GetBodyPartTags() const { return BodyPartTags; }
	
	// Add To
	void AddToXP(int32 InXP);
	void AddToLevel(int32 InLevel);
	void AddToAttributePoints(int32 InPoints);

	// Delegates
	FOnTextValueChangedSignature OnCharacterNameChanged;
	FOnTextureChangedSignature OnCharacterIconChanged;
	FOnInt32ValueChangedSignature OnLevelChanged;
	FOnInt32ValueChangedSignature OnXPChanged;
	FOnTribeDataChangedSignature OnTribeDataChanged;
	FOnInt32ValueChangedSignature OnAttributePointsChanged;
	FOnTagContainerChangedSignature OnBodyPartTagsChanged;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void InitializeComponent() override;
	
private:

	/* Inits random IV's independent of GAS which GAS uses to set IV's*/
	FORCEINLINE void InitializeRandomIVs() { IVSet.SetRandomIVValues(); }
	
	/* Randomly sets the base combat power */
	void InitializeCombatPowerRandomly();
	
	// Mutable
	
	UPROPERTY(EditAnywhere, Category="CharacterContext", meta=(AllowPrivateAccess), ReplicatedUsing=OnRep_CharacterName)
	FText CharacterName;

	/**@note Initial icon is set from editor,
	 * but it would be cool to have the icon changed based on emotion using players voice/facial expressions as an indicator like pizza tower*/
	UPROPERTY(EditAnywhere, Category="CharacterContext", meta=(AllowPrivateAccess), ReplicatedUsing=OnRep_CharacterIcon)
	UTexture2D* CharacterIcon;

	UPROPERTY(EditAnywhere, Category="CharacterContext", meta=(AllowPrivateAccess), ReplicatedUsing=OnRep_TribeData)
	FTribeData TribeData;

	UPROPERTY(EditAnywhere, Category="CharacterContext", meta=(AllowPrivateAccess), ReplicatedUsing=OnRep_XP)
	int32 XP;

	/* Not designer editable */

	// TODO: Set from equipment component!
	/* Temporarily will be decided by the designer,
	 * in the future will be set from the equipment component,
	 * this is context is only used for queries related to checking if a creature meets item requirements */
	UPROPERTY(EditAnywhere, Category="CharacterContext", meta=(GameplayTagFilter="BodyParts", AllowPrivateAccess), Replicated)
	FGameplayTagContainer BodyPartTags;
	
	// Set from XP
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level;

	// Could add biome data later 

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints;

	/* Immutable */

	UPROPERTY(EditAnywhere, Category="CharacterContext", meta=(AllowPrivateAccess, GameplayTagFilter="Genders"), Replicated)
	FGameplayTag Gender;

	/* Temporarily will be decided by the designer, in the future elemental types will be decided based on biome and parts */
	UPROPERTY(EditAnywhere, Category="CharacterContext", meta=(GameplayTagFilter="CreatureTypes", AllowPrivateAccess), Replicated)
	FGameplayTagContainer CreatureTypeTags;

	UPROPERTY(EditAnywhere, Category="CharacterContext", meta=(AllowPrivateAccess), Replicated)
	int32 BaseCombatPower;
	
	UPROPERTY(VisibleAnywhere, Replicated)
	FCharacterCombatValues IVSet;
	
	/* ~End of Immutable~ */
	/* ~End of not designer editable~ */

	/* Combat Power dependent variables */
	
	/** @remark Could make into gameplay tag later if necessary **/
	UPROPERTY(VisibleAnywhere, Replicated)
	EGrowthRate LevelGrowthRate;

	UPROPERTY(VisibleAnywhere, Replicated)
	FGameplayTag Rarity;

	UPROPERTY(VisibleAnywhere, Replicated)
	int32 BaseXP;

	/* ~End of Combat Power dependent variables~ */
	
	// On Rep Functions
	
	UFUNCTION()
	void OnRep_CharacterName();

	UFUNCTION()
	void OnRep_CharacterIcon();
	
	UFUNCTION()
	void OnRep_Level();

	UFUNCTION()
	void OnRep_XP();

	UFUNCTION()
	void OnRep_TribeData();
	
	UFUNCTION()
	void OnRep_AttributePoints();
};