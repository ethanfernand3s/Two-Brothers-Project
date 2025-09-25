#include "Inventory/Items/TB_ItemManifest.h"

#include "Inventory/Actors/ItemPickupActor.h"
#include "Inventory/Components/TBItemComponent.h"
#include "Inventory/Items/TBInventoryItem.h"
#include "UI/Widget/Composite/CompositeBase.h"

UTBInventoryItem* FTB_ItemManifest::Manifest(UObject* NewOuter)
{
	UTBInventoryItem* Item = NewObject<UTBInventoryItem>(NewOuter, UTBInventoryItem::StaticClass());
	Item->SetFromItemManifest(*this);

	for (auto& Fragment : Item->GetItemManifestMutable().GetFragmentsMutable())
	{
		Fragment.GetMutable().Manifest();
	}
	ClearFragments();
	
	return Item;
}

void FTB_ItemManifest::AssimilateInventoryFragments(UCompositeBase* Composite) const
{
	const auto& InventoryItemFragments = GetAllFragmentsOfType<FInventoryItemFragment>();
	for (const auto* Fragment : InventoryItemFragments)
	{
		Composite->ApplyFunction([Fragment](UCompositeBase* Widget)
		{
			Fragment->Assimilate(Widget);
		});
	}
}

void FTB_ItemManifest::SpawnPickupActor(const UObject* WorldContextObject, const FVector& SpawnLocation,
                                        const FRotator& SpawnRotation, const FVector& RotatedForward)
{
	if (!IsValid(PickupActorClass) || !IsValid(WorldContextObject)) return;

	AItemPickupActor* ItemActor = WorldContextObject->GetWorld()->SpawnActor<AItemPickupActor>(PickupActorClass, SpawnLocation, SpawnRotation);
	if (!IsValid(ItemActor)) return;
	check(ItemActor->GetItemComponent());

	FVector LaunchImpulse = RotatedForward * 300.f + FVector::UpVector * 200.f;
	ItemActor->GetMeshComponent()->AddImpulse(LaunchImpulse, NAME_None, true);
	
	ItemActor->GetItemComponent()->InitItemManifest(*this);
}

void FTB_ItemManifest::ClearFragments()
{
	for (auto& Fragment : Fragments)
	{
		Fragment.Reset();
	}
	Fragments.Empty();
}