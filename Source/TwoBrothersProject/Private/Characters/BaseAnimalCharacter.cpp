// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoBrothersProject/Public/Characters/BaseAnimalCharacter.h"


#include "Components/WidgetComponent.h"

#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Animal/AnimalAbilitySystemComponent.h"
#include "AbilitySystem/Animal/AnimalAttributeSet.h"
#include "AI/AnimalAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/PawnExtensionComponent.h"
#include "Player/ParasitePlayerState.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "UI/Widget/StatusBarUserWidget.h"



// Sets default values
ABaseAnimalCharacter::ABaseAnimalCharacter()
{
	AnimalAbilitySystemComponent = CreateDefaultSubobject<UAnimalAbilitySystemComponent>(TEXT("AnimalAbilitySystemComponent"));
	AnimalAbilitySystemComponent->SetIsReplicated(true);
	AnimalAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AnimalAttributeSet = CreateDefaultSubobject<UAnimalAttributeSet>(TEXT("AnimalAttributeSet"));
	AnimalAbilitySystemComponent->AddAttributeSetSubobject(AnimalAttributeSet);

	PawnExt = CreateDefaultSubobject<UPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
}

void ABaseAnimalCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	if (!HasAuthority())
	{
		InitStatusBar();
	}
}

void ABaseAnimalCharacter::UnPossessed()
{
	Super::UnPossessed();

	PawnExt->HandlePlayerUnPossess();

	if (SavedAIController)
	{
		SavedAIController->Possess(this);
	}
}

void ABaseAnimalCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	InitAbilityActorInfo();
	InitStatusBar();

	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("CLIENT: Animal Possessed By Has Ran"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SERVER: Animal Possessed By Has Ran"));
	}
}

void ABaseAnimalCharacter::InitStatusBar()
{
	if (StatusBarWidgetComponent)
	{
		if (UStatusBarUserWidget* StatusBarUserWidget = Cast<UStatusBarUserWidget>(StatusBarWidgetComponent->GetUserWidgetObject()))
		{
			StatusBarUserWidget->SetAttributeSet(AnimalAttributeSet);
		}
	}
}

void ABaseAnimalCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	AController* NewController = Cast<AController>(GetController());
	
	if (AAIController* AIC = Cast<AAIController>(NewController))
	{
		SavedAIController = AIC;
		AnimalAbilitySystemComponent->InitAbilityActorInfo(this, this);
		PawnExt->EnsureInitialAttributeDefaults();
		SetOwner(nullptr);

		if (!HasAuthority()) return;
		AnimalAIController = Cast<AAnimalAIController>(NewController);
		AnimalAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		AnimalAIController->RunBehaviorTree(BehaviorTree);
	}
	else
	{
		if (APlayerController* PC = Cast<APlayerController>(NewController))
		{
			if (AParasitePlayerState* PS = PC->GetPlayerState<AParasitePlayerState>())
			{
				AnimalAbilitySystemComponent-> InitAbilityActorInfo(PS,this);
				SetOwner(PC);
				if (HasAuthority())
				{
					PawnExt->HandlePlayerPossess(PS);
				}
			}
		}
	}
	
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("CLIENT: Animal Possessed By Has Ran"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SERVER: Animal Possessed By Has Ran"));
	}
}

UAbilitySystemComponent* ABaseAnimalCharacter::GetAbilitySystemComponent() const
{
	return AnimalAbilitySystemComponent;
}

float ABaseAnimalCharacter::CanBePossessedBy() const
{
	// TODO: Later on add level checks on player and query enemy and player states / statuses,
	// and check where the player is coming from hole wise to make sure its possible.
	return 1.f;
}
