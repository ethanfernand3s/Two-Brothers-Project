#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "TB_FastArray.generated.h"

class UTBItemComponent;
class UTBInventoryComponent;
class UTBInventoryItem;
class UActorComponent;

/* Inventory Item */
USTRUCT()
struct FTB_InventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FTB_InventoryEntry() {}
	
private:

	friend struct FTB_InventoryFastArray;
	friend UTBInventoryComponent;
	UPROPERTY()
	TObjectPtr<UTBInventoryItem> Item = nullptr;
};

/* Inventory */
USTRUCT()
struct FTB_InventoryFastArray : public FFastArraySerializer
{
	GENERATED_BODY()

	FTB_InventoryFastArray() : OwnerComponent(nullptr) {}
	FTB_InventoryFastArray(UActorComponent* InOwner) : OwnerComponent(InOwner) {}

	// FFastArray contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 /*FinalSize*/);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 /*FinalSize*/);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FTB_InventoryEntry, FTB_InventoryFastArray>(Entries, DeltaParms, *this);
	}

	// Helpers
	TArray<UTBInventoryItem*> GetAllItems() const;
	UTBInventoryItem* AddEntry(UTBInventoryItem* NewItem);
	UTBInventoryItem* AddEntry(UTBItemComponent* ItemComponent);
	void RemoveEntry(UTBInventoryItem* Item);
	
	// TODO: Might need to change this function to return the first item by type and key (something like level or activated ability)
	/* O(n) loop comparing the entries item's tag, return the first matching type */
	UTBInventoryItem* FindFirstItemByType(const FGameplayTag& ItemType);

private:

	/* Inventory Array */
	UPROPERTY()
	TArray<FTB_InventoryEntry> Entries;

	/* Inventory Component */
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FTB_InventoryFastArray> : public TStructOpsTypeTraitsBase2<FTB_InventoryFastArray>
{
	enum { WithNetDeltaSerializer = true };
};
