// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Base/BaseAttributeSet.h"
#include "UObject/Object.h"
#include "Player/Interfaces/PlayerInterface.h"
#include "BaseWidgetController.generated.h"

class IPlayerInterface;
class ATBPlayerController;
class AParasitePlayerState;
class UParasiteAbilitySystemComponent;
class UParasiteAttributeSet;
class UAnimalAbilitySystemComponent;
class UAnimalAttributeSet;
class UUIDataAsset;

USTRUCT (BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	
	FWidgetControllerParams (){};
	
	FWidgetControllerParams(
	ATBPlayerController* InTBPC,
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

UCLASS(Blueprintable)
class TWOBROTHERSPROJECT_API UBaseWidgetController : public UObject
{
	GENERATED_BODY()
public:
	
	void SetWidgetControllerParams(const TUniquePtr<FWidgetControllerParams>& WCParams);
	
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();

	const UUIDataAsset* GetUIDataAsset() const;
	bool IsAnimalInhabited() const;
	UBaseAbilitySystemComponent* GetActiveASC(bool bIsAnimalPriority) const;
	UBaseAttributeSet* GetActiveAS(bool bIsAnimalPriority) const;
	IPlayerInterface* GetActivePI(bool bIsAnimalPriority) const;

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
};

