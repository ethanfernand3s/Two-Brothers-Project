// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoBrothersProject/Public/Characters/BaseAnimalCharacter.h"


#include "AbilitySystem/Animal/AnimalAbilitySystemComponent.h"
#include "AbilitySystem/Animal/AnimalAttributeSet.h"
#include "AI/AnimalAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/AnimalExtensionComponent.h"
#include "Characters/CharacterContextComponent.h"
#include "Characters/Data/Gender.h"
#include "Net/UnrealNetwork.h"
#include "Player/ParasitePlayerState.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "UI/HUD/PlayerHUD.h"



// Sets default values
ABaseAnimalCharacter::ABaseAnimalCharacter()
{
	AnimalAbilitySystemComponent = CreateDefaultSubobject<UAnimalAbilitySystemComponent>(TEXT("AnimalAbilitySystemComponent"));
	AnimalAbilitySystemComponent->SetIsReplicated(true);
	AnimalAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AnimalAttributeSet = CreateDefaultSubobject<UAnimalAttributeSet>(TEXT("AnimalAttributeSet"));
	AnimalAbilitySystemComponent->AddAttributeSetSubobject(AnimalAttributeSet);

	CharacterContextComponent = CreateDefaultSubobject<UCharacterContextComponent>(TEXT("CharacterContextComponent"));
	PawnExt = CreateDefaultSubobject<UAnimalExtensionComponent>(TEXT("PawnExtensionComponent"));

	bIsInitialised = false;
}

void ABaseAnimalCharacter::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Move to OnPossess
	InitActorInfo();
	
	if (PawnExt)
	{
		PawnExt->InitializePossessionWidgets(SocketChances, WidgetClass, GetMesh());
	}
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

void ABaseAnimalCharacter::InitActorInfo()
{
	AController* NewController = Cast<AController>(GetController());
	
	if (AAIController* AIC = Cast<AAIController>(NewController))
	{
		AnimalAbilitySystemComponent->InitAbilityActorInfo(this, this);
		if (!HasAuthority()) return;
		
		// Setup AI
		SavedAIController = AIC;
		SetOwner(nullptr);
		AnimalAIController = Cast<AAnimalAIController>(NewController);
		AnimalAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		AnimalAIController->RunBehaviorTree(BehaviorTree);

		// TODO: Add Loading From Save!!
		LoadProgress();
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

void ABaseAnimalCharacter::LoadProgress()
{
	// TODO: Add Loading From Save!!

	if (!bIsInitialised)
	{
		CharacterContextComponent->InitializeCharacterContext(
		FText::FromString("Neo"),                      // Name
		1,                                             // Level
		0,                                             // XP
		FTribeData(
			FText::FromString("NONE"),					// Tribe Name
			FText::FromString(""),						// Tribe Desc
			nullptr,									// Icon (null for now)
			FLinearColor::Gray							// Tribe Color
		),
		ECharacterGender::Male							// Gender
		);
		
		CharacterContextComponent->InitializeCombatRelatedVars();
		AnimalAbilitySystemComponent->AddIvsToAttributes(CharacterContextComponent->GetIVSet());
		AnimalAbilitySystemComponent->SetBaseStats(CharacterContextComponent->GetBaseCombatPower(), CharacterContextComponent->GetLevel());
		
		bIsInitialised = true;
	}
	
}

void ABaseAnimalCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseAnimalCharacter, bIsInitialised);
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

FPossessionSocketData ABaseAnimalCharacter::FindClosestPossessionSocket(const FVector& TraceImpactPoint) const
{
	//TODO: Could Optimize Move Semantics and Find Faster Algo
	float ClosestDistSqr = TNumericLimits<float>::Max();
	FPossessionSocketData ClosestSocket;

	for (const FPossessionSocketData& SocketData : SocketChances)
	{
		const FVector SocketLocation = GetCurrentSocketLocation(SocketData.SocketGameplayTag);
		const float DistSqr = FVector::DistSquared(TraceImpactPoint, SocketLocation);
		if (DistSqr < ClosestDistSqr)
		{
			ClosestDistSqr = DistSqr;
			ClosestSocket = SocketData;
		}
	}

	return ClosestSocket;
}

FVector ABaseAnimalCharacter::GetCurrentSocketLocation(FGameplayTag SocketName) const
{
	FName FNameSocket  = SocketName.GetTagName();
	return GetMesh()->GetSocketLocation(FNameSocket);
}