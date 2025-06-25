// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

struct FCharacterIVSet;
class UAbilitySystemComponent;
class UGameplayEffect;
class UBiomeDataAsset;
class UWidgetComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TWOBROTHERSPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ABaseCharacter();

protected:
	
	virtual void InitAbilityActorInfo();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void AddIvsToAttributes(UAbilitySystemComponent* ASC, const FCharacterIVSet& CharIvSet);

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> StatusBarWidgetComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly ,Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> ApplyIvsGameplayEffect;
};
