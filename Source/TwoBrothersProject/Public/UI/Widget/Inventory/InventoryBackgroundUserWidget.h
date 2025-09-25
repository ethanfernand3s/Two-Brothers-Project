// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/HoverItemTargetInterface.h"
#include "InventoryBackgroundUserWidget.generated.h"

class UHoverItemUserWidget;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHoverItemDrop, int32 /*RequestedAmount*/);

class UImage;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UInventoryBackgroundUserWidget : public UUserWidget, public IHoverItemTargetInterface
{
	GENERATED_BODY()

public:
	
	virtual void HandleOpposingClick(UHoverItemUserWidget* HoverWidget) override;
	FOnHoverItemDrop OnHoverItemDrop;
	
protected:
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
private:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_DimmedBackground;

};
