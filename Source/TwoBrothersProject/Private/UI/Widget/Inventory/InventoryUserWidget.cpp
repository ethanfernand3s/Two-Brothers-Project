// InventoryUserWidget.cpp

#include "UI/Widget/Inventory/InventoryUserWidget.h"

#include "TBGameplayTags.h"
#include "Blueprint/DragDropOperation.h"
#include "Inventory/Utils/InventoryStatics.h"
#include "UI/Widget/Inventory/InventoryBackgroundUserWidget.h"
#include "UI/Widget/Inventory/CharacterPanel/CharacterPanelUserWidget.h"
#include "UI/Widget/Inventory/CharacterPanel/CharacterDetail/CharacterDetailsUserWidget.h"
#include "UI/Widget/Inventory/Items/HoverItemUserWidget.h"
#include "UI/Widget/Inventory/Slots/SlotPanelUserWidget.h"
#include "UI/WidgetController/BaseWidgetController.h"
#include "UI/WidgetController/InventoryWidgetController.h"

void UInventoryUserWidget::SetWidgetController(UBaseWidgetController* InWidgetController)
{
    Super::SetWidgetController(InWidgetController);
    if (IsValid(CharacterPanel)) { CharacterPanel->SetWidgetController(InWidgetController); }
    if (IsValid(InventoryPanel)) { InventoryPanel->SetWidgetController(InWidgetController); }
}

FSlotAvailabilityResult UInventoryUserWidget::HasRoomForItem(UTBItemComponent* ItemComponent) const
{
    const FGameplayTag ItemCategory(UInventoryStatics::GetItemCategoryFromItemComp(ItemComponent));
    if (const SlotContainerPair* SlotContainerPairPtr = EquippableSlotContainers.Find(ItemCategory))
    {
        // Choose parasite or animal side
        const TWeakObjectPtr<USlotContainerUserWidget>& ChosenSlotContainer =
            (WidgetController->IsAnimalInhabited())
                ? SlotContainerPairPtr->Value
                : SlotContainerPairPtr->Key;
        
        if (ChosenSlotContainer.IsValid())
        {
            if (USlotContainerUserWidget* Container = ChosenSlotContainer.Get())
            {
                const FSlotAvailabilityResult& PreferredContainerResult = Container->HasRoomForItem(ItemComponent);
                if (PreferredContainerResult.SlotAvailabilities.Num() > 0) return PreferredContainerResult;

                // Otherwise desired space isn't available add to inventory instead (if can)
                const FSlotAvailabilityResult& InventoryContainerResult = InventoryPanel->GetSlotContainer()->HasRoomForItem(ItemComponent);
                return InventoryContainerResult;
            }
        }
    }
    else if (ItemCategory.MatchesTag(FGameplayTag::RequestGameplayTag("ItemCategories")))
    {
        /* Note: Inventory can take any ItemCategory tag as long as its of ItemCategories */
        const FSlotAvailabilityResult& InventoryContainerResult = InventoryPanel->GetSlotContainer()->HasRoomForItem(ItemComponent);
        return InventoryContainerResult;
    }
    
    // Doesn't have valid item category
    UE_LOG(LogTemp, Warning, TEXT("Item category is invalid."))
    return FSlotAvailabilityResult();
}

void UInventoryUserWidget::NativeOnInitialized()
{ 
    Super::NativeOnInitialized();

    if (!IsValid(CharacterPanel) || !IsValid(CharacterPanel->GetParasiteDetailsUserWidget())) return;
    SetupParasiteContainers(CharacterPanel->GetParasiteDetailsUserWidget());

    if (!IsValid(InventoryPanel)) return;
    InventoryPanel->GetSlotContainer()->SetContainerOwner(true);
    InventoryPanel->GetSlotContainer()->SetOwningCanvasPanel(CanvasPanel);
    DimmedBackground->OnHoverItemDrop.AddUObject(this, &UInventoryUserWidget::HandleHoverItemDrop, InventoryPanel->GetSlotContainer());
}

bool UInventoryUserWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    // Not Handled by children therefore a fail.
    if (UHoverItemUserWidget* HoverItem = Cast<UHoverItemUserWidget>(InOperation->Payload))
    {
        HoverItem->GetOwningSlotContainer()->DragRejected(HoverItem);
        return true; 
    }

    return false;
}

void UInventoryUserWidget::NativeDestruct()
{
    Super::NativeDestruct();
    ClearSelectedSlots();
}

void UInventoryUserWidget::OnWidgetControllerSet()
{
    ResolveAnimalInhabitance(WidgetController->IsAnimalInhabited());
}

void UInventoryUserWidget::OnWidgetControllerRebound(bool bIsAnimalInhabited)
{
    ResolveAnimalInhabitance(bIsAnimalInhabited);
}

void UInventoryUserWidget::ResolveAnimalInhabitance(bool bIsAnimalInhabited)
{
    const auto& Tags = FTBGameplayTags::Get();
    UCharacterDetailsUserWidget* AnimalDetails = CharacterPanel->GetAnimalDetailsUserWidget();
    if (!IsValid(AnimalDetails)) return;

    if (bIsAnimalInhabited)
    {
        auto AddAnimalSlot = [&](const FGameplayTag& Category, USlotContainerUserWidget* Container)
        {
            if (IsValid(Container))
            {
                SlotContainerPair& Pair = EquippableSlotContainers.FindOrAdd(Category);
                Pair.Value = Container; // assign Animal slot

                Container->SetContainerOwner(false);
                Container->SetOwningCanvasPanel(CanvasPanel);
                // bind delegate w/ handle
                FDelegateHandle Handle = DimmedBackground->OnHoverItemDrop.AddUObject(
                    this, &UInventoryUserWidget::HandleHoverItemDrop, Container);

                AnimalDropDelegateHandles.Add(Category, Handle);
            }
        };

        AddAnimalSlot(Tags.ItemCategories_Abilities_Main,    AnimalDetails->GetMainAbilitySlotContainer());
        AddAnimalSlot(Tags.ItemCategories_Abilities_Default, AnimalDetails->GetDefaultAbilitySlotContainer());
        AddAnimalSlot(Tags.ItemCategories_Abilities_Passive, AnimalDetails->GetPassiveAbilitySlotContainer());
    }
    else
    {
        // Clear animal side of the pair + unbind delegates
        for (auto& HandlePair : AnimalDropDelegateHandles)
        {
            DimmedBackground->OnHoverItemDrop.Remove(HandlePair.Value);

            if (SlotContainerPair* Pair = EquippableSlotContainers.Find(HandlePair.Key))
            {
                Pair->Value = nullptr; // clear Animal slot
            }
        }
        AnimalDropDelegateHandles.Reset();
    }
}


void UInventoryUserWidget::SetupParasiteContainers(const UCharacterDetailsUserWidget* ParasiteDetails)
{
    const auto& Tags = FTBGameplayTags::Get();

    EquippableSlotContainers.FindOrAdd(Tags.ItemCategories_Abilities_Main).Key   = ParasiteDetails->GetMainAbilitySlotContainer();
    EquippableSlotContainers.FindOrAdd(Tags.ItemCategories_Abilities_Default).Key = ParasiteDetails->GetDefaultAbilitySlotContainer();
    EquippableSlotContainers.FindOrAdd(Tags.ItemCategories_Abilities_Passive).Key = ParasiteDetails->GetPassiveAbilitySlotContainer();

    for (auto& EquippableSlotContainer : EquippableSlotContainers)
    {
        const TWeakObjectPtr<USlotContainerUserWidget>& ChosenSlotContainer = EquippableSlotContainer.Value.Key;
        if (ChosenSlotContainer.IsValid())
        {
            ChosenSlotContainer->SetOwningCanvasPanel(CanvasPanel);
            ChosenSlotContainer->SetContainerOwner(true);

            DimmedBackground->OnHoverItemDrop.AddUObject(
                    this, &UInventoryUserWidget::HandleHoverItemDrop, ChosenSlotContainer.Get());
        }
    }
}

void UInventoryUserWidget::HandleHoverItemDrop(int32 AmountToDrop, USlotContainerUserWidget* Container)
{
    if (IsValid(Container) && Container->HasHoverItemActive())
    {
        Container->DropItemFromHoverItem(AmountToDrop);
    }
}

USlotContainerUserWidget* UInventoryUserWidget::GetDestinationContainer(const USlotContainerUserWidget* OriginSlotContainer, const FGameplayTag& ItemCategory) const
{
    if (!IsValid(OriginSlotContainer) || !ItemCategory.IsValid()) return nullptr;
	
    if (OriginSlotContainer->HasCategoryPreference())  return InventoryPanel->GetSlotContainer();
    
    if (const SlotContainerPair* SlotContainerPairPtr = EquippableSlotContainers.Find(ItemCategory))
    {
        // Choose parasite or animal side
        const TWeakObjectPtr<USlotContainerUserWidget>& ChosenSlotContainer =
            (WidgetController->IsAnimalInhabited())
                ? SlotContainerPairPtr->Value
                : SlotContainerPairPtr->Key;
        
        if (ChosenSlotContainer.IsValid())
        {
            if (USlotContainerUserWidget* Container = ChosenSlotContainer.Get())
            {
                return Container;
            }
        }
    }
    
    return nullptr;
}

void UInventoryUserWidget::SetAllGridSlotsCompatibilityTints(bool bIsPickup, const UTBInventoryItem* ItemToCheck)
{
    for (auto& SlotContainerPair : EquippableSlotContainers)
    {
        if (InventoryWidgetController && InventoryWidgetController->IsParasiteFocusedCharacter())
        {
            if (SlotContainerPair.Value.Key.IsValid()) SlotContainerPair.Value.Key->SetGridSlotsCompatibilityTints(bIsPickup, ItemToCheck);
        }
        else
        {
            if (SlotContainerPair.Value.Value.IsValid()) SlotContainerPair.Value.Value->SetGridSlotsCompatibilityTints(bIsPickup, ItemToCheck);
        }
    }
}

void UInventoryUserWidget::AddNewSelectedSlot(USlotContainerUserWidget* OwningSlotContainer, USlotUserWidget* NewestSlot)
{
    SelectedGridSlots.Add({OwningSlotContainer, NewestSlot});
}

void UInventoryUserWidget::EvenlySplitPreviousSelectedSlotStackAmounts(int32 AmountToRemove)
{    
    /* Update any selected items */
    for (auto& SelectedSlotPair : SelectedGridSlots)
    {
        USlotContainerUserWidget* OwningSlotContainer = SelectedSlotPair.Key.Get();
        USlotUserWidget* SelectedSlot = SelectedSlotPair.Value.Get();
        if (IsValid(OwningSlotContainer) && IsValid(SelectedSlot))
        {
            if (AmountToRemove > 0) OwningSlotContainer->RemoveItemAmountFromGrid(AmountToRemove, SelectedSlot->GetTileIndex());
        }
    }
}

int32 UInventoryUserWidget::GetAmountOfPreviousSelectedSlots()
{
    return SelectedGridSlots.Num();
}

void UInventoryUserWidget::ClearSelectedSlots()
{
    SelectedGridSlots.Reset();
}
