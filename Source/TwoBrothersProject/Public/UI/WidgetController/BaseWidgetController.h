// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseWidgetController.generated.h"

class UParasiteAbilitySystemComponent;
class UParasiteAttributeSet;
class UAnimalAbilitySystemComponent;
class UAnimalAttributeSet;
USTRUCT (BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	FWidgetControllerParams (){};
	FWidgetControllerParams (APlayerController* InPC,APlayerState* InPS,
		UParasiteAbilitySystemComponent* InParasiteASC,UParasiteAttributeSet* InParasiteAS,
		UAnimalAbilitySystemComponent* InAnimalASC = nullptr,UAnimalAttributeSet* InAnimalAS = nullptr) :
	TBPlayerController(InPC),ParasitePS(InPS),ParasiteAsc(InParasiteASC),ParasiteAttributeSet(InParasiteAS),AnimalAsc(InAnimalASC),AnimalAttributeSet(InAnimalAS){};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> TBPlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> ParasitePS = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UParasiteAbilitySystemComponent> ParasiteAsc = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UParasiteAttributeSet> ParasiteAttributeSet = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimalAbilitySystemComponent> AnimalAsc = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimalAttributeSet> AnimalAttributeSet = nullptr;
};
UCLASS(Blueprintable)
class TWOBROTHERSPROJECT_API UBaseWidgetController : public UObject
{
	GENERATED_BODY()
public:
	
	void SetWidgetControllerParams(const TUniquePtr<FWidgetControllerParams>& WCParams);
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();
protected:

	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> TBPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> ParasitePS;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UParasiteAbilitySystemComponent> ParasiteASC;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UParasiteAttributeSet> ParasiteAttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAnimalAbilitySystemComponent> AnimalASC;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAnimalAttributeSet> AnimalAttributeSet;

	
};

