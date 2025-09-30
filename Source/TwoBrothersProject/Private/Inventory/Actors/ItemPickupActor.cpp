// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Actors/ItemPickupActor.h"

#include "Components/SphereComponent.h"
#include "Inventory/Components/TBInventoryComponent.h"
#include "Inventory/Components/TBItemComponent.h"
#include "Inventory/Interfaces/InventoryInterface.h"


AItemPickupActor::AItemPickupActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);

	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetEnableGravity(true); 
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Block);
	MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // don’t block pawns

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	SphereComp->SetupAttachment(MeshComp);
	SphereComp->InitSphereRadius(100.f);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true; 
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AItemPickupActor::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	
	ECollisionChannel ItemTraceChannel = ECC_GameTraceChannel1;
	SphereComp->SetCollisionObjectType(ItemTraceChannel);
	
	ItemComponent = FindComponentByClass<UTBItemComponent>();

	SpawnTime = GetWorld()->GetTimeSeconds();
	bCanBePickedUp = false;
	
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		// Allow pickup after 0.5s
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this]()
		{
			bCanBePickedUp = true;
		}, PickupDelay, false);
	});

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::ItemOverlapped);
	
}

void AItemPickupActor::ItemOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bCanBePickedUp) return;

	if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UInventoryInterface::StaticClass()))
	{
		// Set once on server, will replicate to clients and call OnRep_AttractingActor
		AttractingActor = OtherActor;
		bIsFloatingToAttractingPawn = true;
		
		// Immediate local cosmetic on server too
		SetupBobble();
	}
}

void AItemPickupActor::SetupBobble()
{
	// So we can lerp smoothly
	MeshComp->SetSimulatePhysics(false);
	BobbleBaseZ = GetActorLocation().Z + HoverOffset;
}

void AItemPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) return;
	
	if (bIsFloatingToAttractingPawn) TryMagneticFloatToAttractingPawn(DeltaTime);
	else
	{
		// Update bobble base if physics is active
		if (MeshComp->IsSimulatingPhysics())
		{
			FVector Vel = MeshComp->GetPhysicsLinearVelocity();
			// Check if item has basically stopped moving
			if (Vel.SizeSquared() < 1.f)
			{
				SetupBobble();
			}
		}
		else
		{
			TryBobbleFloat(DeltaTime);
		}
	}
}

void AItemPickupActor::TryBobbleFloat(float DeltaTime)
{
	if (MeshComp->IsSimulatingPhysics()) return; // still bouncing

	float Time = GetWorld()->GetTimeSeconds();
	float Bobble = FMath::Sin(Time * BobbleSpeed) * BobbleAmplitude;

	FVector Loc = GetActorLocation();
	Loc.Z = BobbleBaseZ + Bobble;
	SetActorLocation(Loc);

	// Slow spin
	FRotator Rot = GetActorRotation();
	Rot.Yaw += BobbleSpinAnglePerSecond * DeltaTime;
	SetActorRotation(Rot);
}

void AItemPickupActor::TryMagneticFloatToAttractingPawn(float DeltaTime)
{
	if (!IsValid(AttractingActor) || !bCanBePickedUp) return;
	
	FVector TargetLocation = AttractingActor->GetActorLocation();
	FVector ItemLocation = GetActorLocation();
	FVector Distance = TargetLocation - ItemLocation;
	
	const float DistanceSquared = (TargetLocation - ItemLocation).SizeSquared();
	FVector Direction = Distance.GetSafeNormal();
	
	FVector NewVelocity = FMath::Lerp(GetVelocity(), Direction * MaxAttractionSpeed, AttractionStrength);
	FVector NewPosition = ItemLocation + NewVelocity * DeltaTime;
	SetActorLocation(NewPosition, true);
	
	if (DistanceSquared < MinPickupDistanceThresholdSquared)
	{
		// Server gives item
		if (UTBItemComponent* ItemComp = FindComponentByClass<UTBItemComponent>())
		{
			if (const IInventoryInterface* InventoryInterface = Cast<IInventoryInterface>(AttractingActor))
			{
				if (IsValid((InventoryInterface->GetInventoryComponent())))
					InventoryInterface->GetInventoryComponent()->TryAddItem(ItemComp);
				AttractingActor = nullptr;
				bIsFloatingToAttractingPawn = false;
			}
		}
	}
}

// TODO: Setup Chunks and player sweeps for optimal collision
/* Collision for item is temporarily checked in the character */ 