#include "Vehicles/StartGame/CrashLandingShipActor.h"

#include "NiagaraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
#include "Characters/ParasiteCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

ACrashLandingShipActor::ACrashLandingShipActor()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionProfileName(FName("BlockAll"));
	BoxComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetIsReplicated(true);
	
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(BoxComponent);
	ShipMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RocketTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RocketTrail"));
	RocketTrail->SetupAttachment(ShipMesh);
	RocketTrail->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(RocketTrail);
	
	CrashMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("CrashMovement"));
	CrashMovement->UpdatedComponent = BoxComponent; // Root
	CrashMovement->bRotationFollowsVelocity = true;
	
	bReplicates = true;
	AActor::SetReplicateMovement(true);

	SetNetUpdateFrequency(100.f);  // ~100 updates per second (fast)
	SetMinNetUpdateFrequency(30.f);   // fallback if bandwidth is tight
}

void ACrashLandingShipActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(BoxComponent)) BoxComponent->OnComponentHit.AddDynamic(this, &ACrashLandingShipActor::OnColliderHit);
}

void ACrashLandingShipActor::AttachPlayer(AParasiteCharacter* InParasiteCharacter)
{
	if (!IsValid(InParasiteCharacter) || !IsValid(ShipMesh) || !ShipMesh->DoesSocketExist(LandingSocketName)) return;
	
	ParasiteCharacter = InParasiteCharacter;
	ParasiteCharacter->AttachToMeshOrActor(LandingSocketName, ShipMesh, this, true);
	ActivateShip();
}

void ACrashLandingShipActor::ActivateShip() const
{
	CrashMovement->SetVelocityInLocalSpace(FVector::ForwardVector * CrashMovement->InitialSpeed);
	CrashMovement->Activate(true);
}

void ACrashLandingShipActor::OnColliderHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    FVector NormalImpulse, const FHitResult& Hit)
{
	// Only runs on server because of binding done with auth check in begin play
	if (!HasAuthority()) return;
	
    CrashIntoGround(Hit);
}

void ACrashLandingShipActor::CrashIntoGround(const FHitResult& Hit)
{
    if (!ParasiteCharacter.IsValid() || !IsValid(Controller))
    {
    	HandleDestruction(Hit.ImpactPoint);
    	return;
    }
	
    ParasiteCharacter->DetachFromAllActors(this, FDetachmentTransformRules::KeepWorldTransform);
	
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		PC->Possess(ParasiteCharacter.Get());
	}
	
    ParasiteCharacter->ForceNetUpdate();

    // Physics impulse that should actually affect server-auth physics
    if (RadialForceComponent)
    {
        RadialForceComponent->FireImpulse(); // server-side
    }

	HandleDestruction(Hit.ImpactPoint);
}

void ACrashLandingShipActor::HandleDestruction(const FVector_NetQuantize& HitLocation)
{
	// VFX/SFX on everyone
	Multicast_PlayShipDestructionEffects(HitLocation);

	// Destroy on server AFTER multicast (don’t destroy in the multicast)
	SetLifeSpan(0.2f);
}

void ACrashLandingShipActor::Multicast_PlayShipDestructionEffects_Implementation(const FVector_NetQuantize& HitLocation)
{
    // Skip on dedicated servers
    if (IsNetMode(NM_DedicatedServer)) return;

    if (RocketTrail) RocketTrail->Deactivate();

    if (ExplosionEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, HitLocation);
    }
    if (ExplosionSoundEffect)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ExplosionSoundEffect, HitLocation);
    }
}
