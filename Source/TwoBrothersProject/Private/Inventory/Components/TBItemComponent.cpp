// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Components/TBItemComponent.h"

#include "Net/UnrealNetwork.h"


UTBItemComponent::UTBItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UTBItemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
}

void UTBItemComponent::InitItemManifest(FTB_ItemManifest CopyOfManifest)
{
	ItemManifest = CopyOfManifest;
}

void UTBItemComponent::PickedUp()
{
	// Do something cosmetic wise
	
	GetOwner()->Destroy();
}