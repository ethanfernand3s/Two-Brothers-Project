#pragma once
#include "GameplayTagContainer.h"

#include "GridTypes.Generated.h"

class USlotContainerUserWidget;
class UTBInventoryItem;

// Single slot current availability data
USTRUCT()
struct FSlotAvailability
{
	GENERATED_BODY()

	FSlotAvailability() {}
	FSlotAvailability(int32 ItemIndex, int32 Room, bool bHasItem) : Index(ItemIndex), AmountToFill(Room), bItemAtIndex(bHasItem) {}

	/* The index to fill */
	int32 Index{INDEX_NONE};
	/* The amount to fill at that index */
	int32 AmountToFill{0};
	/* Whether there's already an item present at that index */
	bool bItemAtIndex{false};
};

// The plan summary for the whole add operation
USTRUCT()
struct FSlotAvailabilityResult
{
	GENERATED_BODY()

	FSlotAvailabilityResult() {}

	/* The actual item data of the item to add */
	TWeakObjectPtr<UTBInventoryItem> Item;

	/* Preferred type of container to add to */
	FGameplayTag PreferredContainerTag;
	/* Preferred holder */
	bool bPreferParasiteHolder{false};
	/* Total room to fill across multiple slots*/
	int32 TotalRoomToFill{0};
	/* Slots to fill */
	TArray<FSlotAvailability> SlotAvailabilities;
	/* Amount that couldn't be added/picked-up */
	int32 Remainder{0};
	/* Whether the item to add is stackable*/
	bool bStackable{false};
};