#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "TB_ItemFragments.generated.h"

struct FTB_ItemManifest;
class UCompositeBase;
class UBaseGameplayAbility;

/** @remark Every item needs at least an IconFragment, ContextFragment, and RarityFragment*/
USTRUCT(BlueprintType)
struct FTB_ItemFragment
{
	GENERATED_BODY()

	FTB_ItemFragment() {}
	FTB_ItemFragment(const FTB_ItemFragment&) = default;
	FTB_ItemFragment& operator=(const FTB_ItemFragment&) = default;
	FTB_ItemFragment(FTB_ItemFragment&&) = default;
	FTB_ItemFragment& operator=(FTB_ItemFragment&&) = default;
	virtual ~FTB_ItemFragment() {}

	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(FGameplayTag Tag) { FragmentTag = Tag; }

	/* Virtual function to be used when wanting to do something on the manifestation of the item */
	virtual void Manifest() {}
	
private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

/* Item fragment specifically for assimilation into a widget. */
class UInv_CompositeBase;
USTRUCT(BlueprintType)
struct FInventoryItemFragment : public FTB_ItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UCompositeBase* Composite) const;
protected:
	bool MatchesWidgetTag(const UCompositeBase* Composite) const;
};

/**
 * The fragment for any Images relating to an item
 */
USTRUCT(BlueprintType)
struct FImageFragment : public FInventoryItemFragment
{
	GENERATED_BODY()
	
	UTexture2D* GetImage() const { return Image; }
	void SetImage(UTexture2D* InImage) { Image = InImage; }
	void SetImageTint(const FLinearColor& Tint) { ImageTint = Tint; }
	
	virtual void Assimilate(UCompositeBase* Composite) const override;

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UTexture2D> Image{nullptr};
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bOverrideImageDimensions = false;
	
	UPROPERTY(EditAnywhere, Category = "Inventory", meta=(EditConditionHides="bOverrideImageDimensions"))
	FVector2D ImageDimensions{90.f, 90.f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FLinearColor ImageTint = FLinearColor::White;
};

USTRUCT(BlueprintType)
struct FMaterialFragment : public FInventoryItemFragment
{
	GENERATED_BODY()
	
	UMaterialInstance* GetMaterial() const { return Material; }

private:
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	UMaterialInstance* Material = nullptr;
};

/**
 * The fragment for any text relating to an item
 * @remark Must set text color externally if using as rarity color
 */
USTRUCT(BlueprintType)
struct FTextFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FText GetText() const { return FragmentText; }
	void SetText(const FText& Text) { FragmentText = Text; }
	void SetTextColor(const FLinearColor& InTextColor) { FragmentTextColor = InTextColor; }
	virtual void Assimilate(UCompositeBase* Composite) const override;

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText FragmentText;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FLinearColor FragmentTextColor = FLinearColor::White;
};

/**
 * The fragment for the rarity of an item
 * @note Rarity should directly correlate with the spawn-rate
 */
USTRUCT(BlueprintType)
struct FRarityFragment : public FTB_ItemFragment 
{
	GENERATED_BODY()
	
	void SetRarity(FGameplayTag InRarityTag) { RarityTag = InRarityTag; }
	FGameplayTag GetRarity() const { return RarityTag; }

private:

	// The rarity of an item, can be set manually or externally
	UPROPERTY(EditAnywhere, Category = "Inventory",  meta=(GameplayTagFilter="Rarities"))
	FGameplayTag RarityTag = FGameplayTag::EmptyTag;
};

/**
 * The fragment for the tags directly linked to an ability
 *
 * @Remark On Top Of The Regular Requirements Abilities Also Require:
 * AbilityFragment, LevelFragment
 * And Optionally CreatureTypeFragment, And RequirementsFragment
 */
USTRUCT(BlueprintType) 
struct FAbilityFragment : public FTB_ItemFragment
{
	GENERATED_BODY()

	FGameplayTag GetAbilityTag() const { return AbilityTag; }
	FGameplayTag GetAbilityCooldownTag() const { return AbilityCooldownTag; }
	FGameplayTag GetAbilityTypeTag() const { return AbilityTypeTag; }
	TSubclassOf<UBaseGameplayAbility> GetAbilityClass() const { return AbilityClass; }
	
private:

	// The tag that represents the ability
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;

	// The tag to broadcast the abilities cooldown to the UI
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag AbilityCooldownTag = FGameplayTag::EmptyTag;

	// The type of ability
	UPROPERTY(EditAnywhere, Category = "Inventory", meta=(GameplayTagFilter="AbilityTypes"))
	FGameplayTag AbilityTypeTag = FGameplayTag::EmptyTag;

	// The blueprint version of this ability
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UBaseGameplayAbility> AbilityClass = nullptr;
};

/* The fragment for the requirements an item has */
USTRUCT(BlueprintType)
struct FRequirementsFragment : public FTB_ItemFragment
{
	GENERATED_BODY()

	bool MeetsItemRequirements(int32 CreatureLevel, const FGameplayTagContainer& CreatureBodyPartSockets,
		const FGameplayTagContainer& CreatureCreatureTypes) const;
	
	int32 GetLevelRequirement() const { return LevelRequirement; }
	FGameplayTagContainer GetRequiredSocketsTagContainer() const { return RequiredSocketsTag; }
	FGameplayTag GetRequiredCreatureType() const { return RequiredCreatureType; }

private:

	// The required level to use this item
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 LevelRequirement = 1;
	
	// The required socket(s) need to use this ability
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTagContainer RequiredSocketsTag = FGameplayTagContainer::EmptyContainer;

	// The required creature type need to use this ability
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag RequiredCreatureType = FGameplayTag::EmptyTag;
};

/* The fragment for the creature type of an item */
USTRUCT(BlueprintType)
struct FCreatureTypeFragment : public FTB_ItemFragment
{
	GENERATED_BODY()
	
	FGameplayTag GetAbilityCreatureTypeTag() const { return AbilityCreatureTypeTag; }

private:

	// The creature type of the item
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag AbilityCreatureTypeTag = FGameplayTag::EmptyTag;
};

/* The fragment for the level of an item */
USTRUCT(BlueprintType)
struct FLevelFragment : public FTB_ItemFragment
{
	GENERATED_BODY()

	float GetProgressPercentage() const { return (XPRequired != 0) ? (static_cast<float>(CurrentXP) / XPRequired) : 0.0f; }
	int32 GetCurrentLevel() const { return CurrentLevel; }
	int32 GetCurrentXP() const { return CurrentXP; }
	int32 GetXPRequired() const { return XPRequired; }
	
private:

	// The current level of an item
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 CurrentLevel = 1;

	// The current XP of an item
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 CurrentXP = 0;

	// TODO: Should be set from UI DA in correlation with rarities
	// The XP required to get the items next level
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 XPRequired = 0;

	/* Might need to include something to specify what it takes for XP */
};

USTRUCT(BlueprintType)
struct FStackableFragment : public FTB_ItemFragment
{
	GENERATED_BODY()

	int32 GetMaxStackSize() const { return MaxStackSize; }
	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 NewStackCount) { StackCount = NewStackCount; }

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxStackSize{1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 StackCount{1};
};