// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/PlayerHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/OverlayUserWidget.h"
#include "UI/Widget/Inventory/AttributeMenuUserWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	OverlayUserWidget = CreateWidget<UOverlayUserWidget>(GetWorld(), OverlayWidgetClass);
	if (IsValid(OverlayUserWidget))
	{
		OverlayUserWidget->AddToViewport();
	}

	// TODO: Change back to attribute class after setting up
	AttributeMenuUserWidget = CreateWidget<UUserWidget>(GetWorld(), AttributeMenuUserWidgetClass);
	if (IsValid(AttributeMenuUserWidget))
	{
		AttributeMenuUserWidget->AddToViewport();
	}
}
