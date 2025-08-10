#include "Vehicles/StartGame/CrashLandingShipActor.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

ACrashLandingShipActor::ACrashLandingShipActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ShipMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShipMesh"));
	SetRootComponent(ShipMesh);

	bReplicates = true;
}

void ACrashLandingShipActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACrashLandingShipActor::AttachPlayer(APawn* Pawn)
{
	if (!HasAuthority() || !IsValid(Pawn)) return;

	if (ACharacter* Char = Cast<ACharacter>(Pawn))
	{
		Char->GetCharacterMovement()->DisableMovement();
	}

	static const FName AttachSocketName = "LandingSocket";
	if (ShipMesh && ShipMesh->DoesSocketExist(AttachSocketName))
	{
		Pawn->AttachToComponent(ShipMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocketName);

		Multicast_OnPlayerAttached(Pawn);
	}

	Pawn->ForceNetUpdate();
}

void ACrashLandingShipActor::StartCrashSequence()
{
	// Simulate delay before impact (replace with Timeline or Notify if needed)
	GetWorldTimerManager().SetTimerForNextTick(this, &ACrashLandingShipActor::CrashIntoGround);
}

void ACrashLandingShipActor::CrashIntoGround()
{
	if (!HasAuthority()) return;

	// Detach and re-enable player
	APawn* AttachedPawn = Cast<APawn>(ShipMesh->GetAttachParentActor());
	if (AttachedPawn)
	{
		AttachedPawn->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		if (ACharacter* Char = Cast<ACharacter>(AttachedPawn))
		{
			Char->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			Char->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}

	// Visually switch ship mesh
	Multicast_SwitchToBrokenShipMesh();
}

void ACrashLandingShipActor::Multicast_OnPlayerAttached_Implementation(APawn* Pawn)
{
	if (ACharacter* Char = Cast<ACharacter>(Pawn))
	{
		Char->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACrashLandingShipActor::Multicast_SwitchToBrokenShipMesh_Implementation()
{
	if (BrokenShipMesh)
	{
		ShipMesh->SetSkeletalMesh(BrokenShipMesh);
	}
}
