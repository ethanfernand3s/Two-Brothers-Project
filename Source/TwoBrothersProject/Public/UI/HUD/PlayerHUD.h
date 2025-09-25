#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/BaseWidgetController.h"
#include "PlayerHUD.generated.h"

class UInfoMessage;
class UTBInventoryItem;
struct FGameplayTag;
class UTBInventoryComponent;
class UTBEquipSlotsComponent;

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

UCLASS()
class TWOBROTHERSPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:

	/* Called in init actor info */
	void InitUI(ATBPlayerController* PC);
	
	/** @remark Must be called after actor info is initialized*/
	void InitOverlay(ATBPlayerController* PC);
	UOverlayWidgetController* GetOverlayWidgetController(ATBPlayerController* PC);

	/** @remark Must be called after actor info is initialized*/
	UInventoryUserWidget* InitInventory(ATBPlayerController* PC);
	UInventoryWidgetController* GetInventoryWidgetController(ATBPlayerController* PC);

	void SetupWidgetParams(ATBPlayerController* PC);
	
private:
	
	UPROPERTY() TObjectPtr<UOverlayUserWidget> OverlayUserWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widget Classes", meta = (AllowPrivateAccess))
	TSubclassOf<UOverlayUserWidget> OverlayWidgetClass;

	UPROPERTY() TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditDefaultsOnly, Category = "Widget Classes", meta = (AllowPrivateAccess))
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widget Classes", meta = (AllowPrivateAccess))
	TSubclassOf<UInventoryUserWidget> InventoryUserWidgetClass;

	UPROPERTY() TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
	UPROPERTY(EditDefaultsOnly, Category = "Widget Classes", meta = (AllowPrivateAccess))
	TSubclassOf<UInventoryWidgetController>  InventoryWidgetControllerClass;

	TUniquePtr<FWidgetControllerParams> WCParams;
};
