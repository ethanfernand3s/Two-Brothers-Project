// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AnimalExtensionComponent.generated.h"


struct FPossessionSocketData;
class AParasitePlayerState;
class UAbilitySystemComponent;
struct FGameplayAbilitySpecHandle;
class UAnimalAbilitySet;
class UWidgetComponent;
class UPossessionChanceUserWidget;
class USkeletalMeshComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOBROTHERSPROJECT_API UAnimalExtensionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAnimalExtensionComponent();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimalAbilitySet> AnimalAbilitySet;

	void InitializePossessionWidgets(const TArray<FPossessionSocketData>& SocketData, TSubclassOf<UPossessionChanceUserWidget> WidgetClass, USkeletalMeshComponent* MeshComp);
	void UpdatePossessionChance(FName SocketName, float NewChance);
	void HandlePlayerPossess(AParasitePlayerState* OwnerPS);
	void HandlePlayerUnPossess();

protected:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UPROPERTY()
	UAbilitySystemComponent* CachedASC;

	TArray<FGameplayAbilitySpecHandle> GrantedHandles;

	UPROPERTY(Replicated)
	bool bAttributesInitialised;

	UPROPERTY()
	TMap<FName, UWidgetComponent*> SocketWidgetMap;
};
