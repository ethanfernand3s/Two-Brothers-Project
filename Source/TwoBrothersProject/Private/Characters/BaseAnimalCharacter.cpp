// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoBrothersProject/Public/Characters/BaseAnimalCharacter.h"


#include "AbilitySystem/Animal/AnimalAbilitySet.h"
#include "AbilitySystem/Animal/AnimalAbilitySystemComponent.h"
#include "AbilitySystem/Animal/AnimalAttributeSet.h"
#include "AI/AnimalAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CharacterContextComponent.h"
#include "Characters/Data/Gender.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/ParasitePlayerState.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "UI/HUD/PlayerHUD.h"
#include "UI/Widget/Possession/PossessionChanceUserWidget.h"


// Sets default values
ABaseAnimalCharacter::ABaseAnimalCharacter()
{
	AnimalAbilitySystemComponent = CreateDefaultSubobject<UAnimalAbilitySystemComponent>(TEXT("AnimalAbilitySystemComponent"));
	AnimalAbilitySystemComponent->SetIsReplicated(true);
	AnimalAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AnimalAttributeSet = CreateDefaultSubobject<UAnimalAttributeSet>(TEXT("AnimalAttributeSet"));
	AnimalAbilitySystemComponent->AddAttributeSetSubobject(AnimalAttributeSet);

	CharacterContextComponent = CreateDefaultSubobject<UCharacterContextComponent>(TEXT("CharacterContextComponent"));

	bIsInitialised = false;
}

void ABaseAnimalCharacter::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Move to OnPossess
	InitActorInfo();
	InitializePossessionWidgets();
	
}

void ABaseAnimalCharacter::UnPossessed()
{
	Super::UnPossessed();
	
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
				if (!HasAuthority())
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
		ECharacterGender::Male,							// Gender
		0
		);
		
		CharacterContextComponent->InitializeCombatRelatedVars();
		AnimalAbilitySystemComponent->AddIvsToAttributes(CharacterContextComponent->GetIVSet());
		AnimalAbilitySystemComponent->SetBaseStats(CharacterContextComponent->GetBaseCombatPower(), CharacterContextComponent->GetLevel());
		EnsureAbilitiesAreInitialized();
		
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

int32 ABaseAnimalCharacter::GetXP() const
{
	return CharacterContextComponent->GetXP();
}

int32 ABaseAnimalCharacter::GetAttributePointsReward(int32 Level) const
{
	// TODO: Maybe change this to return attribute points reward based on level
	return 1;
}

void ABaseAnimalCharacter::AddToXP(int32 XpToAdd)
{
	CharacterContextComponent->AddToXP(XpToAdd);
}

void ABaseAnimalCharacter::AddToPlayerLevel(int32 LevelsToAdd)
{
	CharacterContextComponent->AddToLevel(LevelsToAdd);
}

void ABaseAnimalCharacter::AddToAttributePoints(int32 AttributePointsToAdd)
{
	CharacterContextComponent->AddToAttributePoints(AttributePointsToAdd);
}

int32 ABaseAnimalCharacter::GetAttributePoints() const
{
	return CharacterContextComponent->GetAttributePoints();
}

int32 ABaseAnimalCharacter::GetLevel()
{
	return CharacterContextComponent->GetLevel();
}

EGrowthRate ABaseAnimalCharacter::GetGrowthRate() const
{
	return CharacterContextComponent->GetGrowthRate();
}

bool ABaseAnimalCharacter::GetIsInhabited() const
{
	return (GetOwner()) ? true : false;
}

int32 ABaseAnimalCharacter::GetBaseXP() const
{
	return CharacterContextComponent->GetBaseXP();
}

float ABaseAnimalCharacter::GetXPMultiplierAmount()
{
	// TODO: Adjust based on current Multiplier Bonuses
	return 1;
};

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

FTransform ABaseAnimalCharacter::GetCurrentSocketTransform(FGameplayTag SocketName) const
{
	FName FNameSocket  = SocketName.GetTagName();
	return GetMesh()->GetSocketTransform(FNameSocket);
}

void ABaseAnimalCharacter::InitializePossessionWidgets()
{
	if (!PossessChanceWidgetClass || !GetMesh()) return;

	for (const FPossessionSocketData& Data : SocketChances)
	{
		const FName SocketName = Data.SocketGameplayTag.GetTagName();
		if (!GetMesh()->DoesSocketExist(SocketName)) continue;

		/* 1.  Create the component with *this* as Outer  */
		UWidgetComponent* WidgetComp =
			NewObject<UWidgetComponent>(this, UWidgetComponent::StaticClass(),
										NAME_None, RF_Transient);

		/* 2.  Attach BEFORE registering so the scene‑graph is correct   */
		WidgetComp->SetupAttachment(GetMesh(), SocketName);

		// TODO: Change this to world space and follow player only when they enter SeekHost (this should be done client side only)
		/* 3.  Configure it                                             */
		WidgetComp->SetWidgetClass(PossessChanceWidgetClass);
		WidgetComp->SetDrawAtDesiredSize(true);
		WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComp->SetActive(false);                 // optional

		/* 4.  Register so it becomes part of the actor at runtime       */
		WidgetComp->RegisterComponent();

		/* 5.  Now the widget instance exists – you can set data         */
		if (UPossessionChanceUserWidget* UI =
				Cast<UPossessionChanceUserWidget>(WidgetComp->GetWidget()))
		{
			UI->SetPossessionChance(Data.PossessionChance);
		}

		SocketWidgetMap.Add(SocketName, WidgetComp);
	}
}

void ABaseAnimalCharacter::UpdatePossessionChance(FName SocketName, float NewChance)
{
	if (UWidgetComponent* WidgetComp = SocketWidgetMap.FindRef(SocketName))
	{
		if (UPossessionChanceUserWidget* Widget = Cast<UPossessionChanceUserWidget>(WidgetComp->GetUserWidgetObject()))
		{
			Widget->SetPossessionChance(NewChance);
		}
	}
}

void ABaseAnimalCharacter::EnsureAbilitiesAreInitialized()
{
	if (!HasAuthority()) return;
	
	AnimalAbilitySystemComponent->AddCharacterAbilities(StartupAbilitySet->Abilities);
	AnimalAbilitySystemComponent->AddCharacterPassiveAbilities(StartupPassiveAbilitySet->Abilities);
}
