// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Utils/InventoryStatics.h"

#include "TBGameplayTags.h"
#include "Inventory/Components/TBInventoryComponent.h"
#include "Inventory/Components/TBItemComponent.h"


UTBInventoryComponent* UInventoryStatics::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;
	UTBInventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UTBInventoryComponent>();
	return InventoryComponent;
}


FGameplayTag UInventoryStatics::GetItemCategoryFromItemComp(const UTBItemComponent* ItemComp)
{
	if (!IsValid(ItemComp)) return FTBGameplayTags::Get().ItemCategories_None;
	return ItemComp->GetItemManifest().GetItemCategory();
}

UInventoryUserWidget* UInventoryStatics::GetInventoryWidget(const APlayerController* PC)
{
	UTBInventoryComponent* IC = GetInventoryComponent(PC);
	if (!IsValid(IC)) return nullptr;

	return IC->GetInventoryWidget();
}