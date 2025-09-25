#include "Inventory/Lists/TB_FastArray.h"
#include "Inventory/Items/TBInventoryItem.h"
#include "Components/ActorComponent.h"
#include "Inventory/Components/TBInventoryComponent.h"
#include "Inventory/Components/TBItemComponent.h"

void FTB_InventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32)
{
	if (UTBInventoryComponent* InventoryComponent = Cast<UTBInventoryComponent>(OwnerComponent); IsValid(InventoryComponent))
	{
		for (int32 Index : RemovedIndices)
		{
			InventoryComponent->OnItemRemoved.Broadcast(Entries[Index].Item);
		}
	}
}

void FTB_InventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32)
{
	if (UTBInventoryComponent* InventoryComponent = Cast<UTBInventoryComponent>(OwnerComponent); IsValid(InventoryComponent))
	{
		for (int32 Index : AddedIndices)
		{
			InventoryComponent->OnItemAdded.Broadcast(Entries[Index].Item);
		}
	}
}

TArray<UTBInventoryItem*> FTB_InventoryFastArray::GetAllItems() const
{
	TArray<UTBInventoryItem*> Out;
	Out.Reserve(Entries.Num());
	for (const auto& E : Entries)
	{
		if (IsValid(E.Item)) Out.Add(E.Item);
	}
	return Out;
}

UTBInventoryItem* FTB_InventoryFastArray::AddEntry(UTBInventoryItem* NewItem)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());
	
	if (!IsValid(NewItem)) return nullptr;
	FTB_InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = NewItem;
	
	MarkItemDirty(NewEntry);
	return NewItem;
}

UTBInventoryItem* FTB_InventoryFastArray::AddEntry(UTBItemComponent* ItemComponent)
{
	check(OwnerComponent);
	if (!IsValid(ItemComponent)) return nullptr;
	
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());
	UTBInventoryComponent* IC = Cast<UTBInventoryComponent>(OwnerComponent);
	if (!IsValid(IC)) return nullptr;

	FTB_InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = ItemComponent->GetItemManifest().Manifest(OwningActor);
	
	IC->AddRepSubObj(NewEntry.Item);
	MarkItemDirty(NewEntry);

	return NewEntry.Item;
}

void FTB_InventoryFastArray::RemoveEntry(UTBInventoryItem* Item)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FTB_InventoryEntry& Entry = *EntryIt;
		if (Entry.Item == Item)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

UTBInventoryItem* FTB_InventoryFastArray::FindFirstItemByType(const FGameplayTag& ItemType)
{
	auto* FoundItem = Entries.FindByPredicate([ItemType = ItemType](const FTB_InventoryEntry& Entry)
	{
		return IsValid(Entry.Item) && Entry.Item->GetItemManifest().GetItemType().MatchesTagExact(ItemType);
	});
	return FoundItem ? FoundItem->Item : nullptr;
}
