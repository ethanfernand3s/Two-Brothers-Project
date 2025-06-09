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
#include "Kismet/GameplayStatics.h"
#include "Player/ParasitePlayerState.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "UI/HUD/PlayerHUD.h"
#include "UI/Widget/ProgressBars/StatusBarUserWidget.h"



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

	// TODO: Move to OnPossess
	InitAbilityActorInfo();
}

void ABaseAnimalCharacter::UnPossessed()
{
	Super::UnPossessed();

	if (!HasAuthority())
	{
		PawnExt->HandlePlayerUnPossess();
	}
	

	if (SavedAIController)
	{
		SavedAIController->Possess(this);
	}
}

void ABaseAnimalCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	InitAbilityActorInfo();

	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("CLIENT: Animal Possessed By Has Ran"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SERVER: Animal Possessed By Has Ran"));
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
		AnimalAbilitySystemComponent->AbilityActorInfoSet();
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
				else
				{
					if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PC->GetHUD()))
					{
						PlayerHUD->InitOverlay(PC);
					}
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

UAnimalAttributeSet* ABaseAnimalCharacter::GetAttributeSet() const
{
	return AnimalAttributeSet;
}

// Currently return 100% if not being possessed and 0% if being possessed
bool ABaseAnimalCharacter::CanBePossessedBy() const
{
	// TODO: Later on add level checks on player and query enemy and player states / statuses,
	// and check where the player is coming from hole wise to make sure its possible.

	
	return !GetOwner();
}
