// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.h"
#include "AbilitySystem/Interfaces/IPossessable.h"
#include "BaseAnimalCharacter.generated.h"

class UCharacterContextComponent;
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
	GENERATED_BODY()

public:

	ABaseAnimalCharacter();

	// ASC Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAnimalAttributeSet* GetAttributeSet() const;
	
	// Possessable Interface
	virtual bool CanBePossessedBy() const override;
	
	FName FindClosestPossessionSocket(const FVector& TraceImpactPoint) const;
	FVector GetSocketLocation(FName SocketName) const;
	bool CanBePossessedAtSocket(FName SocketName) const;
	
	// Put into interface so that animal and parasite both have defined set of rules for having character context
		virtual void LoadProgress();
	//
		
	//UPROPERTY(EditDefaultsOnly)
	//TMap<FName, UPossessionChanceUserWidget*> PossessedSocketMap;
	
protected:

	virtual void BeginPlay() override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;
	virtual void InitAbilityActorInfo() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

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

	// Possession
	UPROPERTY(EditAnywhere)
	TArray<FPossessionSocketData> SocketChances;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPossessionChanceUserWidget> WidgetClass;

	// Additional Components
	UPROPERTY()
	TObjectPtr<UCharacterContextComponent> CharacterContextComponent;

	UPROPERTY()
	TObjectPtr<class AAIController> SavedAIController;
};
