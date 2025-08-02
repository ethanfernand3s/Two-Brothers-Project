// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.h"
#include "Player/Interfaces/PlayerInterface.h"
#include "ParasiteCharacter.generated.h"

class ABaseAnimalCharacter;
class AParasitePlayerState;
class UParasiteAbilitySystemComponent;
UCLASS()
class TWOBROTHERSPROJECT_API AParasiteCharacter : public ABaseCharacter, public IPlayerInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AParasiteCharacter();

	UFUNCTION(NetMulticast, Reliable)
	void SetBurrowCollision(bool bEnable, ABaseAnimalCharacter* TargetAnimal);
	
protected:
	virtual void UnPossessed() override;
	virtual void InitActorInfo() override;

	// Player Interface
	virtual int32 GetXP() const override;
	virtual int32 GetAttributePointsReward(int32 Level) const override;
	virtual void AddToXP(int32 InXP) override;
	virtual void AddToPlayerLevel(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints(int32 InAttributePoints) override;
	virtual int32 GetAttributePoints() const override;
	virtual int32 GetLevel() override;
	virtual EGrowthRate GetGrowthRate() const override;
	virtual bool GetIsInhabited() const override;
	virtual int32 GetBaseXP() const override;
	virtual float GetXPMultiplierAmount() override;
	//~End of Player Interface

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
private:
	UPROPERTY()
	TObjectPtr<UParasiteAbilitySystemComponent> CachedParasiteAbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<AParasitePlayerState> CachedPlayerState;
};
