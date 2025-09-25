#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TB_ItemFragments.h"
#include "StructUtils/InstancedStruct.h"
#include "Inventory/Types/GridTypes.h"
#include "TB_ItemManifest.generated.h"

class UTBInventoryItem;

/**
 * The Item Manifest contains all the necessary data for creating a new Inventory Item
 */
USTRUCT(BlueprintType)
struct FTB_ItemManifest
{
	GENERATED_BODY()

	/* Creates an item and sets its manifest with itself,
	 * then tells all the fragments that they can manifest their own procedural values or do something on the creation of an item*/
	UTBInventoryItem* Manifest(UObject* NewOuter);

	/*  Sets the widget passed in with information from the fragments */
	void AssimilateInventoryFragments(UCompositeBase* Composite) const;

	/*  Spawns a pickup actor and tells it to initialize its manifest */
	void SpawnPickupActor(const UObject* WorldContextObject, const FVector& SpawnLocation = FVector::ZeroVector,
						  const FRotator& SpawnRotation = FRotator::ZeroRotator, const FVector& RotatedForward = FVector::ZeroVector);

	FGameplayTag GetItemCategory() const { return ItemCategory; }
	FGameplayTag GetItemType() const { return ItemType; }
	
	TArray<TInstancedStruct<FTB_ItemFragment>>& GetFragmentsMutable() { return Fragments; }
	
	/* O(n) search throughout all held fragments comparing tag returning fragment instance */
	template<typename T> requires std::derived_from<T, FTB_ItemFragment>
	const T* GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const;

	/* O(n) search throughout all held fragments comparing fragment type returning fragment instance returning fragment instance that's const */
	template<typename T> requires std::derived_from<T, FTB_ItemFragment>
	const T* GetFragmentOfType() const;

	/* O(n) search throughout all held fragments comparing fragment type returning fragment instance that's mutable */
	template<typename T> requires std::derived_from<T, FTB_ItemFragment>
	T* GetFragmentOfTypeMutable();

	/* O(n) search throughout all held fragments comparing fragment type returning all found fragment instances const */
	template<typename T> requires std::derived_from<T, FTB_ItemFragment>
	TArray<const T*> GetAllFragmentsOfType() const;

private:

	/* Resets and empties all the fragments */
	void ClearFragments();
	
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FTB_ItemFragment>> Fragments;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag ItemCategory{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (GameplayTagFilter="GameItems"))
	FGameplayTag ItemType;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<AActor> PickupActorClass;
};

template<typename T>
requires std::derived_from<T, FTB_ItemFragment>
const T* FTB_ItemManifest::GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const
{
	for (const TInstancedStruct<FTB_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(FragmentTag)) continue;
			return FragmentPtr;
		}
	}

	return nullptr;
}

template <typename T> requires std::derived_from<T, FTB_ItemFragment>
const T* FTB_ItemManifest::GetFragmentOfType() const
{
	for (const TInstancedStruct<FTB_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			return FragmentPtr;
		}
	}
	
	return nullptr;
}

template <typename T> requires std::derived_from<T, FTB_ItemFragment>
T* FTB_ItemManifest::GetFragmentOfTypeMutable()
{
	for (TInstancedStruct<FTB_ItemFragment>& Fragment : Fragments)
	{
		if (T* FragmentPtr = Fragment.GetMutablePtr<T>())
		{
			return FragmentPtr;
		}
	}
	
	return nullptr;
}

template <typename T> requires std::derived_from<T, FTB_ItemFragment>
TArray<const T*> FTB_ItemManifest::GetAllFragmentsOfType() const
{
	TArray<const T*> Result;
	for (const TInstancedStruct<FTB_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			Result.Add(FragmentPtr);
		}
	}
	return Result;
}