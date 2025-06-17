// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameFramework/PlayerState.h"
#include "ParasitePlayerState.generated.h"

class UCharacterContextComponent;
class UParasiteAbilitySet;
class UParasiteAttributeSet;
class UParasiteAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API AParasitePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	
	AParasitePlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UParasiteAttributeSet* GetParasiteAttributeSet() const;
	

	void EnsureInitialAttributeDefaults();
	void EnsureAbilitiesAreInitialized();

	const TArray<UGameplayEffect*> GetBuffForHost();
	
	UPROPERTY(Replicated)
	bool bAttributesInitialised;

	UPROPERTY()
	TObjectPtr<UCharacterContextComponent> CharacterContextComponent;
	
	void LoadProgress();
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere,Category="Ability System")
	TObjectPtr<UParasiteAbilitySet> ParasiteAbilitySet;

private:

	// GAS
	UPROPERTY(VisibleAnywhere,Category="Ability System")
	TObjectPtr<UParasiteAbilitySystemComponent> ParasiteAbilitySystem;
	
	TArray<FGameplayAbilitySpecHandle> GrantedHandles;

	UPROPERTY(VisibleAnywhere,Category="Ability System")
	UParasiteAttributeSet* ParasiteAttributeSet;
};
