// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "TBPlayerController.generated.h"


class IPlayerInterface;
class UTBInventoryComponent;
struct FInputBindingHandle;
class UAnimalAttributeSet;
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

	UFUNCTION()
	void OnPawnChanged(APawn* InOldPawn, APawn* InNewPawn);
	
	bool GetIsAnimalPossessed() const;
	TWeakInterfacePtr<IPlayerInterface> GetActivePlayerInterface() const;
	
	UBaseAbilitySystemComponent* GetASC () const;
	UAnimalAttributeSet* GetCurrentAnimalsAttributeSet() const;

	UTBInventoryComponent* GetInventoryComponent() const { return InventoryComponent.Get(); }

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAbilityInputConfig> InputConfig;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> InventoryAction;
	
	
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void Inventory();

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY()
	bool bIsAnimalPossessed;
	
	TWeakObjectPtr<UTBInventoryComponent> InventoryComponent;
};
