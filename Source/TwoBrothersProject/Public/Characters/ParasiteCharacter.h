// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "ParasiteCharacter.generated.h"
class AParasitePlayerState;
class UParasiteAbilitySystemComponent;
UCLASS()
class TWOBROTHERSPROJECT_API AParasiteCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AParasiteCharacter();

	// TODO: Move back to protected
	virtual void LoadProgress() override;
protected:
	virtual void UnPossessed() override;
	virtual void InitAbilityActorInfo() override;
	
	virtual void OnRep_PlayerState() override;


private:
	UPROPERTY()
	TObjectPtr<UParasiteAbilitySystemComponent> ParasiteAbilitySystemComponent;
};
