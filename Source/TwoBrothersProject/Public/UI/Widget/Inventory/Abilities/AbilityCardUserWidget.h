#pragma once

#include "CoreMinimal.h"
#include "AbilityCardUserWidget.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/BaseUserWidget.h"
#include "AbilityCardUserWidget.generated.h"

class USizeBox;
class UHorizontalBox;
class UVerticalBox;
class UInventoryWidgetController;
class UImage;
class UTextBlock;
class UProgressBar;
class UDragDropOperation;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCardTapped,  UAbilityCardUserWidget*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCardDragged, UAbilityCardUserWidget*);

UCLASS()
class TWOBROTHERSPROJECT_API UAbilityCardUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	/* called by controller */
	void SetCardData(const FTBAbilityInfo& InData);
	const FTBAbilityInfo&  GetCardData() const { return CardData; }
	
	bool  bIsInHotbar   = false;
	
	FOnCardTapped  OnCardTapped;
	FOnCardDragged OnCardDragged;

protected:
	/* UWidget overrides */
	virtual FReply NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent&) override;
	virtual void   NativeOnDragDetected   (const FGeometry&, const FPointerEvent&, UDragDropOperation*&) override;
	virtual FReply NativeOnMouseButtonUp  (const FGeometry&, const FPointerEvent&) override;


	/* Helpers for the slot to call */
	void PreviewSizeForSlot(EAbilityType SlotType);   // called on DragEnter
	void RevertPreviewSize();
	
	/* designer‑bound widgets */
	UPROPERTY(meta=(BindWidget))         USizeBox*     SizeBox				 = nullptr;
	UPROPERTY(meta=(BindWidget))         UImage*       Image_Icon			 = nullptr;
	UPROPERTY(meta=(BindWidget))         UImage*       Image_Border_Regular  = nullptr;
	UPROPERTY(meta=(BindWidgetOptional)) UImage*       Image_BorderGlow		 = nullptr;
	UPROPERTY(meta=(BindWidget))         UImage*	   Image_Type			 = nullptr;
	UPROPERTY(meta=(BindWidget))		 UTextBlock*   TextBlock_LevelPrefix = nullptr;
	UPROPERTY(meta=(BindWidget))		 UTextBlock*   TextBlock_LevelNumber = nullptr;
	UPROPERTY(meta=(BindWidget))         UProgressBar* ProgressBar			 = nullptr;
	UPROPERTY(meta=(BindWidget))         UTextBlock*   Textblock_Progress	 = nullptr;
	UPROPERTY(meta=(BindWidgetOptional)) UImage*       Image_HeldBackground  = nullptr;

private:
	/* helpers */
	void UpdateAbilityIcon     (const FTBAbilityInfo& Info);
	void UpdateBorder		   (const FTBAbilityInfo& Info);
	void UpdateText            (const FTBAbilityInfo& Info);
	void UpdateProgress        (const FTBAbilityInfo& Info);
	void UpdateTypes           (const FGameplayTag& Tag);
	void UpdateSize			   (EAbilityType AbilityType);

	UPROPERTY() UInventoryWidgetController* InventoryWidgetController = nullptr;
	UPROPERTY() UMaterialInstanceDynamic*   BorderMID = nullptr;
	UPROPERTY() UMaterialInstanceDynamic*   TextMID   = nullptr;

	FPointerEvent PressedPointer;
	double        PressedTime = 0.0;
	FTBAbilityInfo CardData;
};
