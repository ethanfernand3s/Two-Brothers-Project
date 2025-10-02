// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

#include "AbilitySystem/Animal/AnimalAbilitySet.h"
#include "AbilitySystem/Base/BaseAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

static FName NAME_BaseCharacterCollisionProfile_Capsule(TEXT("Pawn")); // Make BasePawnCapsule Profile
static FName NAME_BaseCharacterCollisionProfile_Mesh(TEXT("NoCollision")); // Make BasePawnMesh Profile

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetNetCullDistanceSquared(900000000.0f);
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionProfileName(NAME_BaseCharacterCollisionProfile_Capsule);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetCollisionProfileName(NAME_BaseCharacterCollisionProfile_Mesh);
	
	StatusBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("StatusBarWidgetComponent");
	StatusBarWidgetComponent->SetRelativeLocation(FVector(0, 0, 100));
	StatusBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	StatusBarWidgetComponent->SetDrawSize(FVector2D(500.f,100.f));
	StatusBarWidgetComponent->SetDrawAtDesiredSize(true);
	StatusBarWidgetComponent->SetupAttachment(RootComponent);
	StatusBarWidgetComponent->SetCastShadow(false);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->TargetArmLength = 350.f;
	SpringArmComponent->SocketOffset = FVector(0.f, 0.f, 100.f);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	
	bReplicates = true;
	SetNetUpdateFrequency(100.f);  // ~100 updates per second (fast)
	SetMinNetUpdateFrequency(30.f);   // fallback if bandwidth is tight
}
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	StatusBarWidgetComponent->SetIsReplicated(false);
	StatusBarWidgetComponent->SetOwnerNoSee(true);
}

void ABaseCharacter::Destroyed()
{
	Super::Destroyed();

	OnDeath();
}

void ABaseCharacter::OnDeath()
{
	// Add custom logic in children if necessary
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitActorInfo();
	UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent());
	if (IsValid(BaseASC) && IsValid(StartupPassiveAbilitySet))
	{
		for (auto& AbilityPair : StartupPassiveAbilitySet->Abilities)
		{
			BaseASC->AddCharacterPassiveAbility(AbilityPair.AbilityClass, AbilityPair.AbilityLevel);
		}
	}
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Only bother on owning client (skip for simulated proxies
	// if you keep ASC in Mixed replication mode)
	if (IsLocallyControlled())
	{
		InitActorInfo();
	}
}


void ABaseCharacter::AttachToMeshOrActor(const FName& RequestedSocketName,
										 USceneComponent* AttachmentComponent, AActor* AttachmentActor,
										 bool ShouldHideCharacter, FAttachmentTransformRules AttachmentRules)
{
	if (!HasAuthority()) return;
	
	if (ShouldHideCharacter) SetActorHiddenInGame(true);
	
	GetCharacterMovement()->DisableMovement();
	
	if (IsValid(AttachmentActor) && RequestedSocketName.IsValid())
	{
		SetAttachmentCollision(true, AttachmentActor);
		// Multicast function because collision isn't auto replicated
		BroadcastAttachmentCollision(true, AttachmentActor);
		
		// Attach to animal
		if (AttachmentComponent)
		{
			AttachToComponent(
				AttachmentComponent,
				AttachmentRules,
				RequestedSocketName);
		}
		else if (AttachmentActor)
		{
			AttachToActor(
					AttachmentActor,
					AttachmentRules,
					RequestedSocketName);
		}

		ForceNetUpdate();
	}
}

void ABaseCharacter::DetachFromAllActors(AActor* DetachmentActor, FDetachmentTransformRules DetachmentRules)
{
	if (!HasAuthority()) return;
	
	SetActorHiddenInGame(false);
	DetachFromActor(DetachmentRules);
	
	// Lift actor up a bit to avoid embedding in ground
	AddActorWorldOffset(FVector(0,0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + DetachmentZOffset));

	// Handle collision
	SetAttachmentCollision(false, DetachmentActor);
	ResetOrientation();
	
	BroadcastAttachmentCollision(false, DetachmentActor); // multicast so clients sync
	
	// Reset movement
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->SetDefaultMovementMode();
		MoveComp->ForceReplicationUpdate();
	}
	
	ForceNetUpdate();
}

FVector ABaseCharacter::GetProjectileCombatSocketLocation()
{
	check(GetMesh());
	return GetMesh()->GetSocketLocation(ProjectileTipSocketName);
}

void ABaseCharacter::BroadcastAttachmentCollision_Implementation(bool bShouldAttach, AActor* Target)
{
	SetAttachmentCollision(bShouldAttach, Target);
	ResetOrientation();
}

void ABaseCharacter::SetAttachmentCollision(bool bShouldAttach, AActor* Target)
{
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (bShouldAttach)
	{
		// Disable Collision
		CapsuleComp ->SetCollisionProfileName(FName("NoCollision"));
		CapsuleComp ->SetEnableGravity(false);

		// Ignore boom probe if you use ECC_Camera:
		CapsuleComp  ->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

		if (Target)
		{
			CapsuleComp->IgnoreActorWhenMoving(Target, true);
		}
	}
	else                       
	{
		// Enable Collision
		CapsuleComp ->SetCollisionProfileName(NAME_BaseCharacterCollisionProfile_Capsule);
		CapsuleComp ->SetEnableGravity(true);

		// Enable Blocking Again
		CapsuleComp  ->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

		if (Target)
		{
			CapsuleComp->IgnoreActorWhenMoving(Target, false);
		}
	}
}

void ABaseCharacter::ResetOrientation()
{
	// Disable so SetActorRotation works
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Apply rotation snap
	SetActorRotation(FRotator::ZeroRotator, ETeleportType::TeleportPhysics);

	if (AController* Contr = GetController())
	{
		Contr->SetControlRotation(FRotator::ZeroRotator);
	}
	
	// Restore flags so normal behavior resumes
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ABaseCharacter::InitActorInfo()
{
	// Implementation in children
}

