// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/PlayerInterface.h"
#include "ParasitePlayerState.generated.h"

class UTBInventoryComponent;
class UCharacterContextComponent;
class UParasiteAbilitySet;
class UParasiteAttributeSet;
class UParasiteAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API AParasitePlayerState : public APlayerState, public IAbilitySystemInterface, public IPlayerInterface
{
	GENERATED_BODY()

public:
	
	AParasitePlayerState();

	void LoadProgress();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UParasiteAttributeSet* GetParasiteAttributeSet() const;
	const TArray<UGameplayEffect*> GetBuffForHost();

	// Player Interface
	virtual UCharacterContextComponent* GetCharacterContextComponent() const override;
	virtual bool GetIsInhabited() const override;
	virtual float GetXPMultiplierAmount() override;
	//~End of Player Interface
	
	UPROPERTY(Replicated)
	bool bIsInitialised;

	UPROPERTY(Replicated)
	bool bIsFirstServerEnter = true;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCharacterContextComponent> CharacterContextComponent;

	UPROPERTY(EditAnywhere,Category="Ability System")
	TObjectPtr<UParasiteAbilitySet> StartupAbilitySet;
	UPROPERTY(EditAnywhere,Category="Ability System")
	TObjectPtr<UParasiteAbilitySet> StartupPassiveAbilitySet;
	
protected:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	
	// GAS
	UPROPERTY(VisibleAnywhere,Category="Ability System")
	TObjectPtr<UParasiteAbilitySystemComponent> ParasiteAbilitySystem;

	UPROPERTY(VisibleAnywhere,Category="Ability System")
	UParasiteAttributeSet* ParasiteAttributeSet;
};
