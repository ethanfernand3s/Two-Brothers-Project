#include "Inventory/Components/TBInventoryComponent.h"

#include "TBGameplayTags.h"
#include "Characters/Components/CharacterContextComponent.h"
#include "Inventory/Actors/ItemPickupActor.h"
#include "Inventory/Components/TBItemComponent.h"
#include "Inventory/Data/StartupItemsDataAsset.h"
#include "Inventory/Types/GridTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/TBPlayerController.h"
#include "UI/Widget/Inventory/InventoryUserWidget.h"


UTBInventoryComponent::UTBInventoryComponent() : InventoryList(this)
{
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	PrimaryComponentTick.bCanEverTick = false;
	bIsInventoryOpen = false;
	bFirstTimePlaying = true;
}

void UTBInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningController = Cast<ATBPlayerController>(GetOwner());
}

void UTBInventoryComponent::SetInventoryWidget(const TObjectPtr<UInventoryUserWidget>& InventoryUserWidgetIn)
{
	InventoryUserWidget = InventoryUserWidgetIn;

	// At this point ASC has been properly initialized
	// if (bFirstTimePlaying) InitStartupItems();
}

void UTBInventoryComponent::InitStartupItems_Implementation()
{
	// TODO: Fix
	/*if (!IsValid(StartupItemsDataAsset)) return;
	
	for (const auto& ItemClass : StartupItemsDataAsset->StartupItemClasses)
	{
		AItemPickupActor* ItemActor =GetWorld()->SpawnActor<AItemPickupActor>(ItemClass);
		if (!IsValid(ItemActor)) return;

		UTBItemComponent* ItemComponent = ItemActor->GetItemComponent();
		check(ItemComponent);
		
		ItemComponent->GetItemManifest().Manifest(this);
		
		// TryAddItem(ItemComponent);
	}*/
}

void UTBInventoryComponent::TryAddItem_Implementation(UTBItemComponent* ItemComponent)
{
	if (!IsValid(ItemComponent) || !IsValid(InventoryUserWidget)) return;
	FSlotAvailabilityResult AvailabilityResult = InventoryUserWidget->HasRoomForItem(ItemComponent);

	UTBInventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemComponent->GetItemManifest().GetItemType());
	AvailabilityResult.Item = FoundItem;
	
	if (AvailabilityResult.TotalRoomToFill == 0)
	{	
		// Just for testing
		//OnNoRoomInInventory.Broadcast();
		return;
	}
	
	if (AvailabilityResult.Item.IsValid() && AvailabilityResult.bStackable)
	{
		// Add stacks to an item that already exists, we just want to update the item not create a new one.
		OnStackChange.Broadcast(AvailabilityResult);
		Server_AddStacksToItem(ItemComponent, AvailabilityResult.TotalRoomToFill, AvailabilityResult.Remainder);
	}
	else if (AvailabilityResult.TotalRoomToFill > 0)
	{
		// Item doesn't exist in the inventory create a new one and update all persistant slots.
		Server_AddNewItem(ItemComponent, AvailabilityResult.bStackable ? AvailabilityResult.TotalRoomToFill : 0, AvailabilityResult.Remainder, AvailabilityResult.PreferredContainerTag);
	}
}

void UTBInventoryComponent::Server_DropItem_Implementation(UTBInventoryItem* Item, int32 StackCount)
{
	if (!IsValid(Item)) return;
	const int32 NewStackCount = Item->GetTotalStackCount() - StackCount;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}

	SpawnDroppedItem(Item, StackCount);
}

void UTBInventoryComponent::SpawnDroppedItem(UTBInventoryItem* Item, int32 StackCount) 
{
	if (!IsValid(Item)) return;
	
	FTB_ItemManifest& ItemManifest = Item->GetItemManifestMutable();
	if (FStackableFragment* StackableFragment = ItemManifest.GetFragmentOfTypeMutable<FStackableFragment>())
	{
		if (StackCount <= 0) return; // Trying to spawn a stackable item with 0 stack count 
		StackableFragment->SetStackCount(StackCount);
	}
	
	const APawn* OwningPawn = OwningController->GetPawn();
	FVector RotatedForward = OwningPawn->GetActorForwardVector();
	RotatedForward = RotatedForward.RotateAngleAxis(FMath::FRandRange(DropSpawnAngleMin, DropSpawnAngleMax), FVector::UpVector);
	FVector SpawnLocation = OwningPawn->GetActorLocation() + RotatedForward * FMath::FRandRange(DropSpawnDistanceMin, DropSpawnDistanceMax);
	const FRotator SpawnRotation = FRotator::ZeroRotator;
	
	ItemManifest.SpawnPickupActor(this, SpawnLocation, SpawnRotation, RotatedForward);
}

void UTBInventoryComponent::Server_TryUnlockItem_Implementation(UTBInventoryItem* Item, bool bIsEquippableSlot, const UCharacterContextComponent* CharacterContext)
{
	const FGameplayTag& PreviousItemStatus = Item->GetItemStatus();
	// bool bIsEquippableSlot = Slot->IsEquippableSlot; // Assuming the slot is already unlocked since this func is meant to deal with the item.
	auto& Tags = FTBGameplayTags::Get();
	if (const FRequirementsFragment* RequirementsFragment = GetFragment<FRequirementsFragment>(Item, Tags.Fragments_Requirements))
	{
		if (RequirementsFragment->MeetsItemRequirements(CharacterContext->GetLevel(),
														CharacterContext->GetBodyPartTags(),
														CharacterContext->GetCreatureTypes()))
		{
			if (bIsEquippableSlot)
			{
				if (PreviousItemStatus != Tags.Status_Equipped) Item->SetItemStatus(Tags.Status_Equipped);
			}
			else
			{
				if (PreviousItemStatus != Tags.Status_Unlocked) Item->SetItemStatus(Tags.Status_Unlocked);
			}
		}
		else
		{
			if (PreviousItemStatus != Tags.Status_Locked) Item->SetItemStatus(Tags.Status_Locked);
		}
	}
	else
	{
		if (bIsEquippableSlot)
		{
			if (PreviousItemStatus != Tags.Status_Equipped) Item->SetItemStatus(Tags.Status_Equipped);
		}
		else
		{
			if (PreviousItemStatus != Tags.Status_Unlocked) Item->SetItemStatus(Tags.Status_Unlocked);
		}
	}
	if (PreviousItemStatus != Item->GetItemStatus()) OnItemStatusChanged.Broadcast(Item);
}

void UTBInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, InventoryList);
}

void UTBInventoryComponent::Server_AddNewItem_Implementation(UTBItemComponent* ItemComponent, int32 StackCount, int32 Remainder, const FGameplayTag& PreferredContainerTag)
{
	if (!IsValid(ItemComponent)) return;
	
	UTBInventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);
	NewItem->SetPreferredSlotContainerTag(PreferredContainerTag);
	NewItem->SetTotalStackCount(StackCount);
	NewItem->SetItemStatus(FTBGameplayTags::Get().Status_Locked);
	
	if (!IsValid(NewItem)) return;
	
	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
	{
		OnItemAdded.Broadcast(NewItem);
	}
	
	// TODO: Tell the Item Component to destroy its owning actor.
	if (Remainder == 0)
	{
		ItemComponent->PickedUp();
	}
	else if (FStackableFragment* StackableFragment = ItemComponent->GetItemManifest().GetFragmentOfTypeMutable<FStackableFragment>())
	{
		StackableFragment->SetStackCount(Remainder);
	}
}

void UTBInventoryComponent::Server_AddStacksToItem_Implementation(UTBItemComponent* ItemComponent, int32 StackCount,
	int32 Remainder)
{
	if (!IsValid(ItemComponent)) return;
	
	const FGameplayTag& ItemType = IsValid(ItemComponent) ? ItemComponent->GetItemManifest().GetItemType() : FGameplayTag::EmptyTag;
	UTBInventoryItem* Item = InventoryList.FindFirstItemByType(ItemType);
	if (!IsValid(Item)) return;

	Item->SetTotalStackCount(Item->GetTotalStackCount() + StackCount);
	
	if (Remainder == 0)
	{
		ItemComponent->PickedUp();
	}
	else if (FStackableFragment* StackableFragment = ItemComponent->GetItemManifest().GetFragmentOfTypeMutable<FStackableFragment>())
	{
		StackableFragment->SetStackCount(Remainder);
	}
}

void UTBInventoryComponent::AddRepSubObj(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}
}

void UTBInventoryComponent::ToggleInventoryDisplay()
{
	// Necessary check since the widget isn't created on begin play 
	if (IsValid(InventoryUserWidget))
	{
		if (bIsInventoryOpen)
		{
			InventoryUserWidget->SetVisibility(ESlateVisibility::Collapsed);
			ConfigureUIForInventory();
			bIsInventoryOpen = false;
		}
		else
		{
			InventoryUserWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ConfigureUIForInventory();
			bIsInventoryOpen = true;
		}
	}

	OnInventoryMenuToggled.Broadcast(bIsInventoryOpen);
}

void UTBInventoryComponent::ConfigureUIForInventory()
{
	if (bIsInventoryOpen)
	{
		OwningController->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		OwningController->SetInputMode(InputMode);
	}
	else
	{
		OwningController->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InventoryUserWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		OwningController->SetInputMode(InputMode);
	}
}