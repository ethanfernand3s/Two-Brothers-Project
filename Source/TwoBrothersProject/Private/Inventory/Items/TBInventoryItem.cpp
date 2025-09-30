#include "Inventory/Items/TBInventoryItem.h"

#include "TBGameplayTags.h"
#include "Inventory/Components/TBInventoryComponent.h"
#include "Net/UnrealNetwork.h"

UTBInventoryItem::UTBInventoryItem()
{
	auto& Tags = FTBGameplayTags::Get();
	PreferredSlotContainerTag = Tags.ItemCategories_None;
	ItemStatusTag = Tags.Status_None;
}

void UTBInventoryItem::SetFromItemManifest(const FTB_ItemManifest& ItemManifestIn)
{
	ItemManifest = FInstancedStruct::Make<FTB_ItemManifest>(ItemManifestIn);
}

bool UTBInventoryItem::IsStackable() const
{
	const FStackableFragment* Stackable = GetItemManifest().GetFragmentOfType<FStackableFragment>();
	return Stackable != nullptr;
}

void UTBInventoryItem::SetOwningInventoryComponent(UTBInventoryComponent* InventoryComponent)
{
	OwningInventoryComp = InventoryComponent;
}

void UTBInventoryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTBInventoryItem, ItemManifest);
	DOREPLIFETIME(UTBInventoryItem, TotalStackCount);
	DOREPLIFETIME(UTBInventoryItem, PreferredSlotContainerTag);
	DOREPLIFETIME(UTBInventoryItem, ItemStatusTag);
}

void UTBInventoryItem::OnRep_ItemStatus() const
{
	OwningInventoryComp->OnItemStatusChanged.Broadcast(this);
}
