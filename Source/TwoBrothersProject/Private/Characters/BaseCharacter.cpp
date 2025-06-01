// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StatusBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("StatusBarWidgetComponent");
	StatusBarWidgetComponent->SetRelativeLocation(FVector(0, 0, 100));
	StatusBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	StatusBarWidgetComponent->SetDrawSize(FVector2D(500.f,100.f));
	StatusBarWidgetComponent->SetDrawAtDesiredSize(true);
	//StatusBarWidgetComponent->SetupAttachment(GetMesh());
	StatusBarWidgetComponent->SetupAttachment(RootComponent);

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

void ABaseCharacter::InitStatusBar()
{
	// Implementation in children
}

void ABaseCharacter::InitAbilityActorInfo()
{
	// Implementation in children
}

