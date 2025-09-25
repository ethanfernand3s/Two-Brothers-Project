// CrashLandingShipActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrashLandingShipActor.generated.h" // must be the last include in the header

class UBoxComponent;
class AParasiteCharacter;
class UNiagaraComponent;
class URadialForceComponent;
class UProjectileMovementComponent;
class USkeletalMesh;

UCLASS()
class TWOBROTHERSPROJECT_API ACrashLandingShipActor : public APawn
{
	GENERATED_BODY()

public:
	ACrashLandingShipActor();
	
	virtual void BeginPlay() override;

	/** Attach a player pawn to the ship (server only). */
	void AttachPlayer(AParasiteCharacter* InParasiteCharacter);

	void ActivateShip() const;
	
	UPROPERTY(EditDefaultsOnly, Category="Niagara")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category="Audio")
	USoundBase* ExplosionSoundEffect;

	/** Socket on the ship mesh used to hold the pawn while flying. */
	UPROPERTY(EditDefaultsOnly, Category="Names")
	FName LandingSocketName = TEXT("Seat");

protected:
	/* ================= Components ================= */

	/** The visual ship mesh (root) and parent for the pawn while flying. */
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true), Category="Components")
	UStaticMeshComponent* ShipMesh;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true), Category="Components")
	UBoxComponent* BoxComponent;
	
	/** Drives the ballistic arc with gravity. */
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true), Category="Components")
	UProjectileMovementComponent* CrashMovement;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true), Category="Components")
	UNiagaraComponent* RocketTrail;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true), Category="Components")
	URadialForceComponent* RadialForceComponent;
	
	/* ================= Cache ================= */

	/** The pawn currently attached to the ship (server authoritative). */
	TWeakObjectPtr<AParasiteCharacter> ParasiteCharacter;

	/* ================= Flow ================= */

	UFUNCTION()
	void OnColliderHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Detach pawn, restore movement/collision, swap mesh. (server only) */
	void CrashIntoGround(const FHitResult& Hit);

	void HandleDestruction(const FVector_NetQuantize& HitLocation);
	/** Runs on all machines to swap to the broken mesh. */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayShipDestructionEffects(const FVector_NetQuantize& HitLocation);
};
