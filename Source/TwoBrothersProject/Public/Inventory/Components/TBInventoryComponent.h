#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/Lists/TB_FastArray.h"
#include "TBInventoryComponent.generated.h"

class UCharacterContextComponent;
struct FSlotAvailabilityResult;
class UStartupItemsDataAsset;
class ATBPlayerController;
class UInventoryUserWidget;
class UTBInventoryItem;

DECLARE_MULTICAST_DELEGATE_OneParam(FInventoryItemChanged, const UTBInventoryItem* /* Item*/);
DECLARE_MULTICAST_DELEGATE(FNoRoomInInventory);
DECLARE_MULTICAST_DELEGATE_OneParam(FStackChange, const FSlotAvailabilityResult&);

DECLARE_MULTICAST_DELEGATE_OneParam(FInventoryMenuToggled, bool /*bOpen*/);

UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent), Blueprintable)
class TWOBROTHERSPROJECT_API UTBInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	
	UTBInventoryComponent();

	UFUNCTION(Server, Reliable)
	void InitStartupItems();
	void ToggleInventoryDisplay();
	// Setters

	// Set after actor info is initialized since inventory relies on the ASC in our case
	void SetInventoryWidget(const TObjectPtr<UInventoryUserWidget>& InventoryUserWidgetIn);
	UInventoryUserWidget* GetInventoryWidget() { return InventoryUserWidget; }
	
	// Getters
	
	// Data
	/* !! Client RPC is temporary for getting working !! */
	UFUNCTION(Client, Reliable)
	void TryAddItem(UTBItemComponent* ItemComponent);

	UFUNCTION(Server, Reliable)
	void Server_AddNewItem(UTBItemComponent* ItemComponent, int32 StackCount, int32 Remainder, const FGameplayTag& PreferredContainerTag);

	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItem(UTBItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	UFUNCTION(Server, Reliable)
	void Server_DropItem(UTBInventoryItem* Item, int32 StackCount);
	void SpawnDroppedItem(UTBInventoryItem* Item, int32 StackCount);

	// Change to UInventorySlot or just query but index and get item/slot info once secure replication is put into place
	// Note: Optional CharacterContext var passed in for deciding between multiple characters
	UFUNCTION(Server, Reliable)
	void Server_UpdateItemStatus(UTBInventoryItem* Item, bool bIsEquippableSlot, const UCharacterContextComponent* CharacterContext = nullptr);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void AddRepSubObj(UObject* SubObj);

	// Delegates 
	FInventoryItemChanged OnItemAdded;
	FInventoryItemChanged OnItemRemoved;
	FInventoryItemChanged OnItemStatusChanged;

	FNoRoomInInventory OnNoRoomInInventory;

	FStackChange OnStackChange;

	FInventoryMenuToggled OnInventoryMenuToggled;

	// TODO: OnXPChanged, OnLevelChanged

protected:
	
	virtual void BeginPlay() override;

private:
	
	void ConfigureUIForInventory();
	
	UPROPERTY(Replicated)
	FTB_InventoryFastArray InventoryList;
	
	UPROPERTY()
	TObjectPtr<UInventoryUserWidget> InventoryUserWidget;

	TWeakObjectPtr<ATBPlayerController> OwningController;

	bool bIsInventoryOpen;
	
	/* Change to get from save game / playerstate */
	bool bFirstTimePlaying;

	UPROPERTY(EditDefaultsOnly)
	UStartupItemsDataAsset* StartupItemsDataAsset;

	/* Spawn Params */
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnAngleMin = -85.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnAngleMax = 85.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnDistanceMin = 10.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnDistanceMax = 50.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float RelativeSpawnElevation = 70.f;
};
