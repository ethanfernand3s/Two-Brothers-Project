#pragma once

#include "CoreMinimal.h"
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
	
	FOnCardTapped  OnCardTapped;
	FOnCardDragged OnCardDragged;

protected:
	/* UWidget overrides */
	virtual FReply NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent&) override;
	virtual void   NativeOnDragDetected   (const FGeometry&, const FPointerEvent&, UDragDropOperation*&) override;
	virtual FReply NativeOnMouseButtonUp  (const FGeometry&, const FPointerEvent&) override;
	
	/* designer‑bound widgets */
	UPROPERTY(meta=(BindWidget))         USizeBox*     SizeBox				 = nullptr;
	UPROPERTY(meta=(BindWidget))         UImage*       Image_Icon			 = nullptr;
	UPROPERTY(meta=(BindWidget))         UImage*       Image_Border			 = nullptr;
	UPROPERTY(meta=(BindWidget))		 UImage*       Image_RarityColor	 = nullptr;
	UPROPERTY(meta=(BindWidget))		 UImage*       Image_BorderGlow		 = nullptr;
	UPROPERTY(meta=(BindWidget))         UImage*	   Image_Type			 = nullptr;
	UPROPERTY(meta=(BindWidget))		 UTextBlock*   TextBlock_LevelPrefix = nullptr;
	UPROPERTY(meta=(BindWidget))		 UTextBlock*   TextBlock_LevelNumber = nullptr;
	UPROPERTY(meta=(BindWidget))         UProgressBar* ProgressBar			 = nullptr;
	// Purple background image
	UPROPERTY(meta=(BindWidget)) UImage*               Image_CardHolder		 = nullptr;

	UPROPERTY(EditAnywhere) UMaterialInstanceDynamic*   IconMID = nullptr;

private:
	/* helpers */
	void UpdateCardImage		   (const FTBAbilityInfo& Info);
	void UpdateText            (const FTBAbilityInfo& Info);
	void UpdateProgress        (const FTBAbilityInfo& Info);
	void UpdateType           (const FGameplayTag& Tag);

	UPROPERTY() UInventoryWidgetController* InventoryWidgetController = nullptr;

	FPointerEvent PressedPointer;
	double        PressedTime = 0.0;
	FTBAbilityInfo CardData;
};
