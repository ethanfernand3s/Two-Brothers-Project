// InventoryUserWidget.h

#pragma once
#include "CoreMinimal.h"
#include "CharacterPanel/CharacterDetail/CharacterDetailsUserWidget.h"
#include "Inventory/Types/GridTypes.h"
#include "Slots/SlotContainerUserWidget.h"
#include "Slots/SlotPanelUserWidget.h"
#include "UI/Widget/BaseUserWidget.h"
#include "InventoryUserWidget.generated.h"

class USlotPanelUserWidget;
class UInventoryBackgroundUserWidget;
class UCharacterPanelUserWidget;
class UTBItemComponent;
class UInventoryWidgetController;
class UAbilityCardUserWidget;
class USlotContainerUserWidget;

UCLASS()
class TWOBROTHERSPROJECT_API UInventoryUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void SetWidgetController(UBaseWidgetController* InWidgetController) override;

	/** Returns an FSlotAvailabilityResult containing the slot info needed for adding an item. */
	FSlotAvailabilityResult HasRoomForItem(UTBItemComponent* ItemComponent) const;
	
	USlotContainerUserWidget* GetSlotContainer() const { return InventoryPanel->GetSlotContainer(); }
	USlotContainerUserWidget* GetDestinationContainer(const USlotContainerUserWidget* OriginSlotContainer,
														  const FGameplayTag& ItemCategory) const;

	void SetAllGridSlotsCompatibilityTints(bool bIsPickup, const UTBInventoryItem* ItemToCheck = nullptr);
	
	void EvenlySplitPreviousSelectedSlotStackAmounts(int32 AmountToRemove);
	int32 GetAmountOfPreviousSelectedSlots();
	void ClearSelectedSlots();
	void AddNewSelectedSlot(USlotContainerUserWidget* OwningSlotContainer, USlotUserWidget* NewestSlot);
	
protected:
	
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeDestruct() override;
	virtual void OnWidgetControllerSet() override;
	virtual void OnWidgetControllerRebound(bool bIsAnimalInhabited) override;
	
private:
	
	void ResolveAnimalInhabitance(bool bIsAnimalInhabited);
	void SetupParasiteContainers(const UCharacterDetailsUserWidget* ParasiteDetails);
	void HandleHoverItemDrop(int32 AmountToDrop, USlotContainerUserWidget* Container);
	// Widget Controller
	UPROPERTY() UInventoryWidgetController* InventoryWidgetController = nullptr;
	
	// Panels

	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* CanvasPanel = nullptr;
	
	UPROPERTY(meta=(BindWidget))
	USlotPanelUserWidget* InventoryPanel = nullptr;
	
	UPROPERTY(meta=(BindWidget))
	UCharacterPanelUserWidget*  CharacterPanel = nullptr;

	// Background
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UInventoryBackgroundUserWidget> DimmedBackground = nullptr;

	// Cached Vals
	typedef TPair<TWeakObjectPtr<USlotContainerUserWidget> /*ParasiteContainer*/, TWeakObjectPtr<USlotContainerUserWidget> /*AnimalContainer*/> SlotContainerPair;
	TMap<FGameplayTag, SlotContainerPair> EquippableSlotContainers;
	TArray<TPair<TWeakObjectPtr<USlotContainerUserWidget>, TWeakObjectPtr<USlotUserWidget>>> SelectedGridSlots;
	
	/** Delegate handles for Animal slot containers so we can unbind later */
	TMap<FGameplayTag, FDelegateHandle> AnimalDropDelegateHandles;
};
