// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseWidgetController.generated.h"


class AParasitePlayerState;
class ATBPlayerController;
class UParasiteAbilitySystemComponent;
class UParasiteAttributeSet;
class UAnimalAbilitySystemComponent;
class UAnimalAttributeSet;
USTRUCT (BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	FWidgetControllerParams (){};
	FWidgetControllerParams (ATBPlayerController* PC,AParasitePlayerState* PS,
		UParasiteAbilitySystemComponent* PASC,UParasiteAttributeSet* ParasiteAS,
		UAnimalAbilitySystemComponent* AASC,UAnimalAttributeSet* AnimalAS) :
	TBPlayerController(PC),ParasitePS(PS),ParasiteAsc(PASC),ParasiteAttributeSet(ParasiteAS),AnimalAsc(AASC),AnimalAttributeSet(AnimalAS){};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ATBPlayerController> TBPlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AParasitePlayerState> ParasitePS = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UParasiteAbilitySystemComponent> ParasiteAsc = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UParasiteAttributeSet> ParasiteAttributeSet = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimalAbilitySystemComponent> AnimalAsc = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimalAttributeSet> AnimalAttributeSet = nullptr;
};
UCLASS()
class TWOBROTHERSPROJECT_API UBaseWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
protected:

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<ATBPlayerController> TBPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AParasitePlayerState> ParasitePS;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UParasiteAbilitySystemComponent> ParasiteASC;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UParasiteAttributeSet> ParasiteAttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAnimalAbilitySystemComponent> AnimalASC;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAnimalAttributeSet> AnimalAttributeSet;

	
};

