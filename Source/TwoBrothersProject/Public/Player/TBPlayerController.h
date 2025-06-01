// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TBPlayerController.generated.h"

class UBaseAbilitySystemComponent;
class UAbilityInputConfig;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API ATBPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	
	ATBPlayerController();
	
	UFUNCTION(BlueprintImplementableEvent)
	void DamageSelf();
	
protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> InputContext;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAbilityInputConfig> InputConfig;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;
	
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UBaseAbilitySystemComponent> BaseAbilitySystemComponent;
	UBaseAbilitySystemComponent* GetASC () ;
};
