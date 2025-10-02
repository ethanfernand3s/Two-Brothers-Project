// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Base/BaseAttributeSet.h"
#include "UObject/Object.h"
#include "Player/Interfaces/PlayerInterface.h"
#include "BaseWidgetController.generated.h"

struct FTribeData;
class IPlayerInterface;
class ATBPlayerController;
class AParasitePlayerState;
class UParasiteAbilitySystemComponent;
class UParasiteAttributeSet;
class UAnimalAbilitySystemComponent;
class UAnimalAttributeSet;
class UUIDataAsset;

// Character Context Delegates
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRebind, bool bIsAnimalInhabited);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnIntChangedWithSourceSignature, int32,  bool bIsParasiteVal);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFloatChangedWithSourceSignature, float, bool bIsParasiteVal);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTextChangedWithSourceSignature, const FText&, bool bIsParasiteVal);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTribeDataChangedWithSourceSignature, const FTribeData& TribeData, bool bIsParasiteVal);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTextureChangedWithSourceSignature, UTexture2D*, bool bIsParasiteVal);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTagChangedWithSourceSignature, const FGameplayTag&, bool bIsParasiteVal);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCreatureTypesChangedWithSourceSignature, const FGameplayTagContainer&, bool bIsParasiteVal);

USTRUCT (BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	
	FWidgetControllerParams (){};
	
	FWidgetControllerParams(ATBPlayerController* InTBPC,
							AParasitePlayerState* InPS,
							UParasiteAbilitySystemComponent* InParasiteASC,
							UParasiteAttributeSet* InParasiteAS,
							IPlayerInterface* InParasitePI,
							UAnimalAbilitySystemComponent* InAnimalASC = nullptr,
							UAnimalAttributeSet* InAnimalAS = nullptr,
							IPlayerInterface* InAnimalPI = nullptr);
	
	TWeakObjectPtr<ATBPlayerController> TBPC = nullptr;
	TWeakObjectPtr<AParasitePlayerState> ParasitePS = nullptr;
	TWeakObjectPtr<UParasiteAbilitySystemComponent> ParasiteASC = nullptr;
	TWeakObjectPtr<UParasiteAttributeSet> ParasiteAS = nullptr;
	TWeakInterfacePtr<IPlayerInterface> ParasitePI = nullptr;
	TWeakObjectPtr<UAnimalAbilitySystemComponent> AnimalASC = nullptr;
	TWeakObjectPtr<UAnimalAttributeSet> AnimalAS = nullptr;
	TWeakInterfacePtr<IPlayerInterface> AnimalPI = nullptr;
};

// holds handles to the character context values that change
struct FCharacterContextHandles
{
	FDelegateHandle AttributePointsChangedHandle;
	FDelegateHandle LevelChangedHandle;
	FDelegateHandle XPChangedHandle;
	FDelegateHandle CharacterNameChangedHandle;
	FDelegateHandle CharacterIconChangedHandle;
	FDelegateHandle TribeDataChangedHandle;
};

UCLASS(Blueprintable)
class TWOBROTHERSPROJECT_API UBaseWidgetController : public UObject
{
	GENERATED_BODY()
public:
	
	void SetWidgetControllerParams(const TUniquePtr<FWidgetControllerParams>& WCParams);
	
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();
	virtual void RebindToDependencies();
	virtual void UnBindCallbacks();
	void UnbindAnimalCharacterContext();

	void SetIsParasiteFocusedCharacter(bool InbIsParasiteFocusedCharacter) {bIsParasiteFocusedCharacter = InbIsParasiteFocusedCharacter;}
	bool IsParasiteFocusedCharacter() const {return bIsParasiteFocusedCharacter;}
	const UUIDataAsset* GetUIDataAsset() const;
	bool IsAnimalInhabited() const;
	UBaseAbilitySystemComponent* GetActiveASC(bool bIsAnimalPriority) const;
	UBaseAttributeSet* GetActiveAS(bool bIsAnimalPriority) const;
	IPlayerInterface* GetActivePI(bool bIsAnimalPriority) const;

	FOnRebind OnWidgetControllerRebound;
	
							/* Character Context Delegates */
	// Updating Vars
	FOnIntChangedWithSourceSignature			   OnAttributePointsChangedDelegate;
	FOnIntChangedWithSourceSignature			   OnLevelChangedDelegate;
	FOnFloatChangedWithSourceSignature		       OnXPChangedDelegate;
	FOnTextChangedWithSourceSignature              OnCharacterNameChangedDelegate;
	FOnTextureChangedWithSourceSignature		   OnCharacterIconChanged;
	FOnTribeDataChangedWithSourceSignature         OnTribeDataChangedDelegate;

	// One time Broadcast
	FOnTagChangedWithSourceSignature               OnGenderSetDelegate;
	FOnCreatureTypesChangedWithSourceSignature     OnCreatureTypesSetDelegate;
	FOnTagChangedWithSourceSignature               OnRaritySetDelegate;
	
protected:

								/* Widget Param Set*/
	TWeakObjectPtr<ATBPlayerController> TBPC = nullptr;
	TWeakObjectPtr<AParasitePlayerState> ParasitePS = nullptr;
	TWeakObjectPtr<UParasiteAbilitySystemComponent> ParasiteASC = nullptr;
	TWeakObjectPtr<UParasiteAttributeSet> ParasiteAS = nullptr;
	TWeakInterfacePtr<IPlayerInterface> ParasitePI = nullptr;
	TWeakObjectPtr<UAnimalAbilitySystemComponent> AnimalASC = nullptr;
	TWeakObjectPtr<UAnimalAttributeSet> AnimalAS = nullptr;
	TWeakInterfacePtr<IPlayerInterface> AnimalPI = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category = "UI")
	TObjectPtr<UUIDataAsset> UIDataAsset;

							/* Animal Delegate Handles */
	// For attributes
	TMap<FGameplayAttribute, FDelegateHandle> AnimalAttributeHandles;
	// For context
	TOptional<FCharacterContextHandles> AnimalContextHandles;
	bool bIsParasiteFocusedCharacter = true;

private:

	static float GetXPBarPercent(int32 XPAmount, const UCharacterContextComponent* Context);
										/* Character Context */
	void BroadcastCharacterContext(UCharacterContextComponent* CharacterContextComponent, bool bIsParasiteVal) const;
	void BindAllCharacterContext(UCharacterContextComponent* CharacterContextComponent, bool bIsParasiteVal);
};

