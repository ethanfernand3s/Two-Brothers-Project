// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TBGameInstance.h"

#include "Input/InputProcessors/InventoryInputProcessor.h"

void UTBGameInstance::Init()
{
	Super::Init();

	// Inventory UI setup
	FSlateApplication::Get().SetDragTriggerDistance(DragTriggerDistance);
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().RegisterInputPreProcessor(MakeShared<FInventoryInputProcessor>());
	}
}
