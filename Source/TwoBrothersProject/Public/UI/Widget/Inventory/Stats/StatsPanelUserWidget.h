#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/Data/CharacterCombatValues.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/BaseUserWidget.h"
#include "StatsPanelUserWidget.generated.h"

class UUniformGridPanel;
struct FTBAbilityInfo;
class UHorizontalBox;
enum class ECreatureType : uint8;
enum class ERarity : uint8;
class UVerticalBox;
enum class ECharacterGender : uint8;
class UPassiveAbilityDropdownUserWidget;
class UStatsRadarUserWidget;
struct FCharacterCombatValues;
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
class TWOBROTHERSPROJECT_API UStatsPanelUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:

	virtual void SetWidgetController(UObject* InWidgetController) override;

#pragma region Ability Bar

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> UniformGridPanel_AbilitySlots;
	
#pragma endregion Ability Bar
	
#pragma region Passive Ability Bar
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPassiveAbilityDropdownUserWidget> PassiveAbilityDropdown;

	/** Expand/collapse animation */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ExpandPassiveBarAnim;

#pragma endregion Passive Ability Bar
	
#pragma region Stats

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatsRadarUserWidget> StatsRadar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (BindWidget))
	TObjectPtr<UStatButton> StatButton_Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (BindWidget))
	TObjectPtr<UStatButton> StatButton_Strength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (BindWidget))
	TObjectPtr<UStatButton> StatButton_Defense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (BindWidget))
	TObjectPtr<UStatButton> StatButton_AuraStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (BindWidget))
	TObjectPtr<UStatButton> StatButton_AuraDefense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (BindWidget))
	TObjectPtr<UStatButton> StatButton_Speed;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnStatBoostAnim;
	UPROPERTY(EditAnywhere, Category = "Audio")
	TObjectPtr<USoundBase> StatBoostSound;
	
#pragma endregion Stat Buttons
	
#pragma region Character Context Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_TribeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_AttributePoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo",meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_Types;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo",meta = (BindWidget))
	TObjectPtr<UImage> Image_Gender;

#pragma endregion Character Context Variables

protected:
	virtual void NativeConstruct() override;
	virtual void OnWidgetControllerSet() override;
	
private:

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
	
	UPROPERTY()
	TMap<FGameplayTag, FStatUIBinding> MappedStatBindings;
	
#pragma region Attribute Listeners
	
	void SetSingleStat(const FTBAttributeInfo& AttributeInfo);
	void SetHealthStat(const FTBAttributeInfo& CurrentAttributeInfo, const FTBAttributeInfo& MaxAttributeInfo);
	void SetType(const TPair<ECreatureType, ECreatureType>& CreatureTypes);

#pragma endregion Attribute Listeners
	
#pragma region Character Context Listeners

	// Currently immutable might change
	void OnGenderSet(const ECharacterGender& InGender);
	void OnAuraColorSet(const FColor& InColor);
	
	void OnAttributePointsChanged(int NewAttributePoints);
	void OnCharacterNameChanged(const FText& NewCharacterName);
	void OnLevelChanged(int32 NewLevel);
	void OnTribeDataChanged(const FText& NewTribeText);

#pragma endregion Character Context Listeners

#pragma region Stat Button Functions

	UFUNCTION()
	void OnStatButtonPressed_Health();
	UFUNCTION()
	void OnStatButtonPressed_Strength();
	UFUNCTION()
	void OnStatButtonPressed_Defense();
	UFUNCTION()
	void OnStatButtonPressed_AuraStrength();
	UFUNCTION()
	void OnStatButtonPressed_AuraDefense();
	UFUNCTION()
	void OnStatButtonPressed_Speed();
	
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

#pragma region Ability Bar

	void UpdateAbilitySlots(const FTBAbilityInfo& InData);
#pragma endregion Ability Bar
};
