// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
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
}


void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	Super::OnRep_PlayerState();

	// Only bother on owning client (skip for simulated proxies
	// if you keep ASC in Mixed replication mode)
	if (IsLocallyControlled())
	{
		InitAbilityActorInfo();
		InitStatusBar(); 
	}
}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// Skip dedicated servers
	if (GetNetMode() == NM_DedicatedServer) return;

	// Get this *client’s* camera
	FVector   CamLoc;
	FRotator  CamRot;
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->GetPlayerViewPoint(CamLoc, CamRot);

		const FVector WidgetLoc = StatusBarWidgetComponent->GetComponentLocation();
		const FRotator LookAt   = UKismetMathLibrary::FindLookAtRotation(WidgetLoc, CamLoc);
		
		StatusBarWidgetComponent->SetWorldRotation(LookAt);
	}

	
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	StatusBarWidgetComponent->SetIsReplicated(false);
	StatusBarWidgetComponent->SetOwnerNoSee(true);             
}

void ABaseCharacter::InitStatusBar()
{
	// Implementation in children
}

void ABaseCharacter::InitAbilityActorInfo()
{
	// Implementation in children
}

