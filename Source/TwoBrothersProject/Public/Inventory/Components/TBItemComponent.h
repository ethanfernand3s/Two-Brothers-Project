// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/Items/TB_ItemManifest.h"
#include "TBItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class TWOBROTHERSPROJECT_API UTBItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UTBItemComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FTB_ItemManifest GetItemManifest() const { return ItemManifest; }

	// TODO: Make a special world space widget to have for the pickup message pop (similar to borderlands 2 gun pickup-popup
	// (Using the same material that the status bar will use)
	// This function will be called from the item when setting the text for its widget
	// (Might need to be more dynamic because its it will be more than just text, look into fragments
	FString GetPickupMessage() const { return PickupMessage; }

	void InitItemManifest(FTB_ItemManifest CopyOfManifest);
	void PickedUp();

private:
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	FString PickupMessage;

	UPROPERTY(Replicated, EditAnywhere, Category="Inventory")
	FTB_ItemManifest ItemManifest;
};
