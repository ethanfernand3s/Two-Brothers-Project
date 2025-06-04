// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.h"
#include "AbilitySystem/Interfaces/IPossessable.h"
#include "BaseAnimalCharacter.generated.h"

class AAnimalAIController;
class UBehaviorTree;
class UPawnExtensionComponent;
class UAnimalAttributeSet;
class UAnimalAbilitySystemComponent;


UCLASS()
class TWOBROTHERSPROJECT_API ABaseAnimalCharacter : public ABaseCharacter, public IAbilitySystemInterface, public IPossessable
{
public:
	

private:
	GENERATED_BODY()

public:

	ABaseAnimalCharacter();

	// ASC Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Possessable Interface
	virtual bool CanBePossessedBy() const override;

	UPROPERTY()
	TObjectPtr<class AAIController> SavedAIController;
	
protected:

	virtual void BeginPlay() override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;
	

	virtual void InitStatusBar() override;
	virtual void InitAbilityActorInfo() override;

	// GAS
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UPawnExtensionComponent> PawnExt;
	
	UPROPERTY()
	TObjectPtr<UAnimalAbilitySystemComponent> AnimalAbilitySystemComponent;

	UPROPERTY()
	UAnimalAttributeSet* AnimalAttributeSet;

	// AI 
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAnimalAIController> AnimalAIController;
};
