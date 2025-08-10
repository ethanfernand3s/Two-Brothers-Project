#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrashLandingShipActor.generated.h"

UCLASS()
class TWOBROTHERSPROJECT_API ACrashLandingShipActor : public AActor
{
	GENERATED_BODY()

public:
	ACrashLandingShipActor();

protected:
	virtual void BeginPlay() override;

public:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* ShipMesh;

	// Broken ship mesh to swap to
	UPROPERTY(EditDefaultsOnly, Category = "Crash Landing")
	USkeletalMesh* BrokenShipMesh;

	// Attach player to ship
	UFUNCTION(BlueprintCallable)
	void AttachPlayer(APawn* Pawn);

	// Start the crash sequence
	UFUNCTION(BlueprintCallable)
	void StartCrashSequence();

	// Finish the crash: detach player and swap mesh
	UFUNCTION(BlueprintCallable)
	void CrashIntoGround();

	// Replicate player attach visuals
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnPlayerAttached(APawn* Pawn);

	// Replicate mesh swap
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SwitchToBrokenShipMesh();
};
