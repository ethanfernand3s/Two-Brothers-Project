// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.h"
#include "Inventory/Interfaces/InventoryInterface.h"
#include "Player/Interfaces/PlayerInterface.h"
#include "ParasiteCharacter.generated.h"

class ATBPlayerController;
class APlayerHUD;
class ABaseAnimalCharacter;
class AParasitePlayerState;
class UParasiteAbilitySystemComponent;
UCLASS()
class TWOBROTHERSPROJECT_API AParasiteCharacter : public ABaseCharacter, public IPlayerInterface, public IAbilitySystemInterface, public IInventoryInterface
{
	GENERATED_BODY()
public:
	
	AParasiteCharacter();

	// Inventory Interface
	virtual UTBInventoryComponent* GetInventoryComponent() const override;

	// Player Interface
	virtual UCharacterContextComponent* GetCharacterContextComponent() const override;
	virtual bool GetIsInhabited() const override;
	virtual float GetXPMultiplierAmount() override;
	//~End of Player Interface

	// Ability System Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	
	virtual void UnPossessed() override;
	virtual void InitActorInfo() override;

private:
	
	// Cached Vars
	TWeakObjectPtr<UParasiteAbilitySystemComponent> CachedParasiteAbilitySystemComponent;
	TWeakObjectPtr<AParasitePlayerState> CachedPlayerState;
	TWeakObjectPtr<ATBPlayerController> CachedPlayerController;
	TWeakObjectPtr<APlayerHUD> CachedPlayerHUD;
};
