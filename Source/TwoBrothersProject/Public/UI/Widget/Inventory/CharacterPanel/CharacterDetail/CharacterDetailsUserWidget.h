// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "UI/Widget/Inventory/Slots/SlotPanelUserWidget.h"
#include "CharacterDetailsUserWidget.generated.h"

class USlotPanelUserWidget;
class UCanvasPanel;
class UButton;
class USlotContainerUserWidget;
class USlotUserWidget;
class UUniformGridPanel;
class UHorizontalBox;
class UVerticalBox;
class UStatsRadarUserWidget;
class UInventoryWidgetController;
class UTextBlock;
class UStatButton;
class UStatsRadarWidget;
class UWidgetAnimation;
class USoundBase;
class UImage;

USTRUCT()
struct FStatUIBinding
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UStatButton> StatButton = nullptr;
	
	int32 RadarIndex = INDEX_NONE;
};

UCLASS()
class TWOBROTHERSPROJECT_API UCharacterDetailsUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	
	USlotContainerUserWidget* GetMainAbilitySlotContainer()    const { return MainAbility_SlotContainer;        }
	USlotContainerUserWidget* GetDefaultAbilitySlotContainer() const { return DefaultAbilities_SlotContainer;   }
	USlotContainerUserWidget* GetPassiveAbilitySlotContainer() const { return PassivePanel->GetSlotContainer(); }

#pragma region Attribute Listeners
	
	void SetSingleStat(const FTBAttributeInfo& AttributeInfo);
	void SetHealthStat(const FTBAttributeInfo& CurrentAttributeInfo, const FTBAttributeInfo& MaxAttributeInfo);
	void SetType(const FGameplayTagContainer& CreatureTypeTags);

#pragma endregion Attribute Listeners
	
#pragma region Character Context Listeners

	// Currently immutable might change
	void OnGenderSet(const FGameplayTag& InGenderTag);
	void OnRaritySet(const FGameplayTag& InGenderTag);
	
	void OnAttributePointsChanged(int NewAttributePoints);
	void OnCharacterNameChanged(const FText& NewCharacterName);
	void OnLevelChanged(int32 NewLevel);
	void OnTribeDataChanged(const FText& NewTribeText);

#pragma endregion Character Context Listeners
	
protected:
	
	virtual void NativeOnInitialized() override;
	virtual void OnWidgetControllerSet() override;

private:
	
#pragma region Ability Bar
	
	UPROPERTY(meta = (BindWidget))
	USlotContainerUserWidget* MainAbility_SlotContainer;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	USlotContainerUserWidget* DefaultAbilities_SlotContainer;

#pragma endregion Ability Bar
	
#pragma region Passive Ability Bar
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlotPanelUserWidget> PassivePanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_EquippablePassiveAbilitiesContainerToggle;
	
	/** Expand/collapse animation */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ExpandPassiveBarAnim;

#pragma endregion Passive Ability Bar
	
#pragma region Stats

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatsRadarUserWidget> StatsRadar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatButton> StatButton_Health;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatButton> StatButton_Strength;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatButton> StatButton_Defense;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatButton> StatButton_AuraStrength;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatButton> StatButton_AuraDefense;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatButton> StatButton_Speed;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnStatBoostAnim;
	UPROPERTY(EditAnywhere, Category = "Audio")
	TObjectPtr<USoundBase> StatBoostSound;
	
#pragma endregion Stat Buttons
	
#pragma region Character Context Variables

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_CharacterName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Level;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_TribeName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_AttributePoints;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_Types;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Gender;

#pragma endregion Character Context Variables

#pragma region Data
	
	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
	
	UPROPERTY()
	TMap<FGameplayTag, FStatUIBinding> MappedStatBindings;

#pragma endregion Data

#pragma region Stat Button Functions
	
	void OnStatButtonPressed(const FGameplayTag& StatTag) const;
	
	void SetupStatButtons();
	void UpdateAllButtons(bool bShouldEnable);
	void PlayStatIncreasedEffects();

#pragma endregion Stat Button Functions

#pragma region CachedVals

	float PreviousMaxHealth = 0;
	
#pragma endregion CachedVals

#pragma region Passive Bar

	UFUNCTION()
	void ToggleDropdown();

	/** Tracks current expanded state */
	bool bExpanded = false;

#pragma endregion Passive Bar
};
