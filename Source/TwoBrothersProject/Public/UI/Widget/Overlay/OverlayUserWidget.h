#pragma once

#include "CoreMinimal.h"
#include "../BaseUserWidget.h"
#include "OverlayUserWidget.generated.h"

class UAbilityCardDisplay;
class UOverlayWidgetController;
class USlotContainerUserWidget;
class UTextBlock;
class UImage;
class UProgressBar;

UCLASS()
class TWOBROTHERSPROJECT_API UOverlayUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void OnWidgetControllerSet() override;

private:
	
	void OnAttributePointsUpdated(int32 NewAttributePoints, bool bIsParasiteVal);
	void OnCharacterIconChanged(UTexture2D* CharacterIcon, bool bIsParasiteVal);
	void OnLevelUpdated(int32 NewLevel);
	void OnXPPercentageUpdated(float NewXPPercentage);

	void OnHealthUpdated(float NewHealth);
	void OnMaxHealthUpdated(float NewMaxHealth);
	void OnEnergyUpdated(float NewEnergy);
	void OnMaxEnergyUpdated(float NewMaxEnergy);

	void HideSecondaryInfo() const;
	
#pragma region ProgressBars
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Health;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Energy;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_XP;
#pragma endregion ProgressBars

#pragma region Ability Bar
	UPROPERTY(meta = (BindWidget))
	USlotContainerUserWidget* MainAbility_SlotContainer;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	USlotContainerUserWidget* DefaultAbilities_SlotContainer;
#pragma endregion Ability Bar

#pragma region TextBlock
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Level;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PointsToSpend_Primary;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PointsToSpend_Secondary;

#pragma endregion TextBlock

#pragma region Image
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon_Primary;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon_Primary_Fill;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon_Secondary;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon_Secondary_Fill;

#pragma endregion Image

#pragma region Info

	UPROPERTY() UOverlayWidgetController* OverlayWidgetController = nullptr;

#pragma endregion Info
	
	bool bIsAttributePointsEmpty = true;
	
	float Health = 100;
	float MaxHealth = 100;
	float Energy = 100;
	float MaxEnergy = 100;
};
