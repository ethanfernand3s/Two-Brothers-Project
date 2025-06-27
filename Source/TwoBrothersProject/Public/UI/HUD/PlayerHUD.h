// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/BaseWidgetController.h"
#include "PlayerHUD.generated.h"

class UPossessMiniGameUserWidget;
class ATBPlayerController;
class AParasitePlayerState;
class UParasiteAttributeSet;
class UParasiteAbilitySystemComponent;
class UAnimalAbilitySystemComponent;
class UAnimalAttributeSet;
class UOverlayWidgetController;
struct FWidgetControllerParams;
class UInventoryUserWidget;
class UInventoryWidgetController;
class UOverlayUserWidget;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	UOverlayWidgetController* GetOverlayWidgetController(APlayerController* PC);
	UInventoryWidgetController* GetInventoryWidgetController( APlayerController* PC);

	void SetupWidgetParams(APlayerController* PC);
	
	void InitOverlay(APlayerController* PC);
	void Inventory(APlayerController* PC);

	void ConfigureUIForInventory(APlayerController* PC, bool bIsInViewport);

protected:
	
	virtual void BeginPlay() override;
	
private:

	UPROPERTY()
	TObjectPtr<UOverlayUserWidget> OverlayUserWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widget Classes", meta = (AllowPrivateAccess))
	TSubclassOf<UOverlayUserWidget> OverlayWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditDefaultsOnly, Category = "Widget Classes", meta = (AllowPrivateAccess))
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UInventoryUserWidget> InventoryUserWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Classes", meta = (AllowPrivateAccess))
	TSubclassOf<UInventoryUserWidget> InventoryUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Classes", meta = (AllowPrivateAccess))
	TSubclassOf<UInventoryWidgetController>  InventoryWidgetControllerClass;

	TUniquePtr<FWidgetControllerParams> WCParams;
};
