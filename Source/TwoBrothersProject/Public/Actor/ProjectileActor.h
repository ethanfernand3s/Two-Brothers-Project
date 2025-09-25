// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileActor.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
UCLASS()
class TWOBROTHERSPROJECT_API AProjectileActor : public AActor
{
	GENERATED_BODY()

public:

	AProjectileActor();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
protected:

	virtual void BeginPlay() override;
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromASweep,const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
};
 