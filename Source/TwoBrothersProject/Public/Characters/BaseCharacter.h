// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UAnimalAbilitySet;
class UDeathDropComponent;
class UTBInventoryComponent;
class UTBEquipSlotsComponent;
struct FCharacterIVSet;
class UAbilitySystemComponent;
class UGameplayEffect;
class UBiomeDataAsset;
class UWidgetComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TWOBROTHERSPROJECT_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	
	GENERATED_BODY()

public:

	ABaseCharacter();

	/** Connect this characters root to either a mesh component or an actor
	 * @remark Must be called from the server! */
	void AttachToMeshOrActor(const FName& RequestedSocketName,
							 USceneComponent* AttachmentComponent, AActor* AttachmentActor,
							 bool ShouldHideCharacter = false,
							 FAttachmentTransformRules AttachmentRules =
								FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	/** Detach this characters root from any actor that it's currently attached to 
	 * @remark Must be called from the server! */
	void DetachFromAllActors(AActor* DetachmentActor,
							 FDetachmentTransformRules DetachmentRules =
								FDetachmentTransformRules::KeepWorldTransform);

	//interface for return projectile socket spawn point

	virtual FVector GetProjectileCombatSocketLocation();

	// Ability System Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {return nullptr;} // Override in children

	// Start abilities that shouldn't be items for some reason
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimalAbilitySet> StartupPassiveAbilitySet;

	UPROPERTY(EditDefaultsOnly)
	FName ProjectileTipSocketName;
protected:
	
	virtual void InitActorInfo();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void OnDeath();

	

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> StatusBarWidgetComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true), Category = "Detachment")
	float DetachmentZOffset{1.f};
	
	
private:

	/** Configs collision for attachment
	 * @remark Helper for Attaching/Detaching Functions */
	void SetAttachmentCollision(bool bShouldAttach, AActor* Target);
	
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastAttachmentCollision(bool bShouldAttach, AActor* Target);

	void ResetOrientation();
};
