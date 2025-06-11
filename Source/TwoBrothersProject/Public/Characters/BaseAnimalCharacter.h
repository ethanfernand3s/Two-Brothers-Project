// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.h"
#include "AbilitySystem/Interfaces/IPossessable.h"
#include "BaseAnimalCharacter.generated.h"

class UPossessionChanceUserWidget;
class AAnimalAIController;
class UBehaviorTree;
class UAnimalExtensionComponent;
class UAnimalAttributeSet;
class UAnimalAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FPossessionSocketData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName SocketName;

	UPROPERTY(EditAnywhere)
	float PossessionChance = 0.f;
};

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
	UAnimalAttributeSet* GetAttributeSet() const;
	// Possessable Interface
	virtual bool CanBePossessedBy() const override;

	UPROPERTY()
	TObjectPtr<class AAIController> SavedAIController;
	//UPROPERTY(EditDefaultsOnly)
	//TMap<FName, UPossessionChanceUserWidget*> PossessedSocketMap;
protected:

	virtual void BeginPlay() override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;
	
	virtual void InitAbilityActorInfo() override;

	// GAS
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UAnimalExtensionComponent> PawnExt;
	
	UPROPERTY()
	TObjectPtr<UAnimalAbilitySystemComponent> AnimalAbilitySystemComponent;

	UPROPERTY()
	UAnimalAttributeSet* AnimalAttributeSet;

	// AI 
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAnimalAIController> AnimalAIController;

	UPROPERTY(EditAnywhere)
	TArray<FPossessionSocketData> SocketChances;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPossessionChanceUserWidget> WidgetClass;


	
};
