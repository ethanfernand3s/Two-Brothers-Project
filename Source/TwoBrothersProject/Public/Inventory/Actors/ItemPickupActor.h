// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemPickupActor.generated.h"

class USphereComponent;
class UTBItemComponent;

UCLASS()
class TWOBROTHERSPROJECT_API AItemPickupActor : public AActor
{
	GENERATED_BODY()

public:
	
	AItemPickupActor();
	UTBItemComponent* GetItemComponent() const { return ItemComponent.Get(); }
	UStaticMeshComponent* GetMeshComponent() const { return MeshComp; }
	bool CanBePickedUp() const { return bCanBePickedUp; }
	void SetupBobble();

	void TryBobbleFloat(float DeltaTime);
	void TryMagneticFloatToAttractingPawn(float DeltaTime);
	
protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION()
	void ItemOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<USphereComponent> SphereComp;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* MeshComp;
	
	TWeakObjectPtr<UTBItemComponent> ItemComponent;
	
	// Who we’re currently attracted to
	UPROPERTY()
	AActor* AttractingActor = nullptr;
	
	float SpawnTime{0.f};
	float PickupDelay{.5f};
	float MinPickupDistanceThresholdSquared{2.f};
	bool bCanBePickedUp{false};
	bool bIsFloatingToAttractingPawn{false};

	UPROPERTY()
	float BobbleBaseZ{0.f};
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), Category="Bobble")
	float HoverOffset = 10.f;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), Category="Bobble")
	float BobbleSpeed{3.f}; // Frequency
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), Category="Bobble")
	float BobbleAmplitude{4.f}; // Amplitude
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), Category="Bobble")
	float BobbleSpinAnglePerSecond{30.f};

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), Category="Attraction")
	float AttractionStrength = 0.45f;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), Category="Attraction")
	float MaxAttractionSpeed  = 1000.f;
};
