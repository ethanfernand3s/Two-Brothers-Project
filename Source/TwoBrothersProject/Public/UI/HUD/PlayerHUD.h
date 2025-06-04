// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UAttributeMenuUserWidget;
class UOverlayUserWidget;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

	protected:

	virtual void BeginPlay() override;
private:

	UPROPERTY()
	TObjectPtr<UOverlayUserWidget> OverlayUserWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Classes", meta = (AllowPrivateAccess))
	TSubclassOf<UOverlayUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> AttributeMenuUserWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Classes", meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> AttributeMenuUserWidgetClass;
};
