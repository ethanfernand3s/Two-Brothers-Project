#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TB_ItemManifest.h"
#include "TBInventoryItem.generated.h"

UCLASS()
class TWOBROTHERSPROJECT_API UTBInventoryItem : public UObject
{
	GENERATED_BODY()
public:

	UTBInventoryItem();
	
	/* Creates new instance of the item manifest */
	void SetFromItemManifest(const FTB_ItemManifest& ItemManifestIn);

	/* Accessors for the items manifest */
	const FTB_ItemManifest& GetItemManifest() const { return ItemManifest.Get<FTB_ItemManifest>(); }
	FTB_ItemManifest& GetItemManifestMutable() { return ItemManifest.GetMutable<FTB_ItemManifest>(); }

	bool IsStackable() const;
	int32 GetTotalStackCount() const { return TotalStackCount; }
	void SetTotalStackCount(int32 Count) { TotalStackCount = Count; }

	FGameplayTag GetPreferredSlotContainerTag() const {return PreferredSlotContainerTag;}
	void SetPreferredSlotContainerTag(const FGameplayTag& Tag) { PreferredSlotContainerTag = Tag; }

	FGameplayTag GetItemStatus() const {return ItemStatusTag;}
	void SetItemStatus(const FGameplayTag& Tag) { ItemStatusTag = Tag; }
	
	/* Needed for networking */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& Out) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }
	
private:

	UPROPERTY(VisibleAnywhere, Replicated,  meta = (BaseStruct = "/Script/TwoBrotherProject.TB_ItemManifest"))
	FInstancedStruct ItemManifest;

	/* This data is temporary while in the inventory, only item manifest is copied to pickup actor */
	UPROPERTY(Replicated)
	FGameplayTag PreferredSlotContainerTag = FGameplayTag::EmptyTag;
	UPROPERTY(Replicated)
	FGameplayTag ItemStatusTag = FGameplayTag::EmptyTag;
	UPROPERTY(Replicated)
	int32 TotalStackCount{0};
};

/* O(n) search for getting the desired fragment from the items manifest by using a gameplay tag */
template<typename FragmentType>
const FragmentType* GetFragment(const UTBInventoryItem* Item, const FGameplayTag& Tag)
{
	if (!IsValid(Item)) return nullptr;
	return Item->GetItemManifest().GetFragmentOfTypeWithTag<FragmentType>(Tag);
}