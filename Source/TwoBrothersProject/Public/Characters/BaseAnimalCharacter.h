// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Interfaces/IPossessable.h"
#include "Player/Interfaces/PlayerInterface.h"
#include "BaseAnimalCharacter.generated.h"


class UAnimalAbilitySet;
class UCharacterContextComponent;
class UPossessionChanceUserWidget;
class AAnimalAIController;
class UBehaviorTree;
class UAnimalAttributeSet;
class UAnimalAbilitySystemComponent;

USTRUCT(BlueprintType)
 struct FPossessionSocketData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FGameplayTag SocketGameplayTag;

	UPROPERTY(EditAnywhere)
	float PossessionChance = 0.f;
};

UCLASS()
class TWOBROTHERSPROJECT_API ABaseAnimalCharacter : public ABaseCharacter, public IAbilitySystemInterface,
	public IPossessable, public IPlayerInterface
{
	GENERATED_BODY()

public:

	ABaseAnimalCharacter();

	// ASC Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAnimalAttributeSet* GetAttributeSet() const;
	//~End of ASC Interface
	
	// Possessable Interface
	virtual bool CanBePossessedBy() const override;
	//~End Of Possessable Interface

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
	
	FPossessionSocketData FindClosestPossessionSocket(const FVector& TraceImpactPoint) const;
	FVector GetCurrentSocketLocation(FGameplayTag SocketName) const;
	FTransform GetCurrentSocketTransform(FGameplayTag SocketName) const;

	// TODO: Put into interface (Maybe Save Game Interface) so that animal and parasite both have defined set of rules for having character context
		virtual void LoadProgress();
	//
		
	UPROPERTY(Replicated)
	bool bIsInitialised;

	UPROPERTY()
	TObjectPtr<UCharacterContextComponent> CharacterContextComponent;
	
protected:

	virtual void BeginPlay() override;
	virtual void UnPossessed() override;
	virtual void InitActorInfo() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
		
	// GAS
	
	UPROPERTY(EditAnywhere)
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
	TSubclassOf<UPossessionChanceUserWidget> PossessChanceWidgetClass;

	// Additional Components

	UPROPERTY()
	TObjectPtr<class AAIController> SavedAIController;

	// Start Abilities
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimalAbilitySet> StartupAbilitySet;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimalAbilitySet> StartupPassiveAbilitySet;
	
private:

	void InitializePossessionWidgets();
	void UpdatePossessionChance(FName SocketName, float NewChance);
	void EnsureAbilitiesAreInitialized();
	
	UPROPERTY()
	TMap<FName, UWidgetComponent*> SocketWidgetMap;
};
