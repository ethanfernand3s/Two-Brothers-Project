// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponent.generated.h"


class AParasitePlayerState;
class UAbilitySystemComponent;
struct FGameplayAbilitySpecHandle;
class UAnimalAbilitySet;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOBROTHERSPROJECT_API UPawnExtensionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPawnExtensionComponent();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimalAbilitySet> AnimalAbilitySet;

	void HandlePlayerPossess(AParasitePlayerState* OwnerPS);
	void HandlePlayerUnPossess();
	void EnsureInitialAttributeDefaults();

protected:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UPROPERTY()
	UAbilitySystemComponent* CachedASC;

	TArray<FGameplayAbilitySpecHandle> GrantedHandles;

	UPROPERTY(Replicated)
	bool bAttributesInitialised;
};
