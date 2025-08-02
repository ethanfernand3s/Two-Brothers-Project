// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ParasiteCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Parasite/ParasiteAbilitySystemComponent.h"
#include "Characters/BaseAnimalCharacter.h"
#include "Characters/CharacterContextComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/ParasitePlayerState.h"
#include "UI/HUD/PlayerHUD.h"


class AParasitePlayerState;

AParasiteCharacter::AParasiteCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AParasiteCharacter::SetBurrowCollision_Implementation(bool bEnable, ABaseAnimalCharacter* TargetAnimal)
{
	UCapsuleComponent* ParasiteCapsuleComponent = GetCapsuleComponent();
	USkeletalMeshComponent* MeshComponent = GetMesh();

	if (bEnable)               // ← restore normal on eject / success
	{
		// Enable Collision
		ParasiteCapsuleComponent ->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ParasiteCapsuleComponent ->SetEnableGravity(true);
		MeshComponent->SetEnableGravity(true);

		// Enable Blocking Again
		ParasiteCapsuleComponent  ->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
		MeshComponent ->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

		if (TargetAnimal)
		{
			ParasiteCapsuleComponent->IgnoreActorWhenMoving(TargetAnimal, false);
		}
	}
	else                       // ← disable when attaching
	{
		// Disable Collision
		ParasiteCapsuleComponent ->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ParasiteCapsuleComponent ->SetEnableGravity(false);
		MeshComponent->SetEnableGravity(false);

		// Ignore boom probe if you use ECC_Camera:
		ParasiteCapsuleComponent  ->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		MeshComponent ->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

		if (TargetAnimal)
		{
			ParasiteCapsuleComponent->IgnoreActorWhenMoving(TargetAnimal, true);
		}
	}
}

void AParasiteCharacter::UnPossessed()
{
	Super::UnPossessed();
	//Play anim of attaching to host
}

void AParasiteCharacter::InitActorInfo()
{
	CachedPlayerState = GetPlayerState<AParasitePlayerState>();
	check(CachedPlayerState);
	
	if (CachedPlayerState)
	{
		if (CachedParasiteAbilitySystemComponent = Cast<UParasiteAbilitySystemComponent>(CachedPlayerState->GetAbilitySystemComponent()))
		{
			CachedParasiteAbilitySystemComponent->InitAbilityActorInfo(CachedPlayerState,this);
			
			if (HasAuthority())
			{
				CachedPlayerState->LoadProgress();
			}
			
			if (IsLocallyControlled())
			{
				APlayerController* PlayerController = GetController<APlayerController>();
				if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
				{
					PlayerHUD->InitOverlay(PlayerController);
				}
			}
		}
	}

	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("CLIENT: Parasites Possessed By Has Ran"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SERVER: Parasites Possessed By Has Ran"));
	}
}

int32 AParasiteCharacter::GetXP() const
{
	return CachedPlayerState->CharacterContextComponent->GetXP();
}

int32 AParasiteCharacter::GetAttributePointsReward(int32 Level) const
{
	// TODO: Maybe change this to return attribute points reward based on level
	return 1;
}

void AParasiteCharacter::AddToXP(int32 XpToAdd)
{
	CachedPlayerState->CharacterContextComponent->AddToXP(XpToAdd);
}

void AParasiteCharacter::AddToPlayerLevel(int32 LevelsToAdd)
{
	CachedPlayerState->CharacterContextComponent->AddToLevel(LevelsToAdd);
}

void AParasiteCharacter::AddToAttributePoints(int32 AttributePointsToAdd)
{
	CachedPlayerState->CharacterContextComponent->AddToAttributePoints(AttributePointsToAdd);
}

int32 AParasiteCharacter::GetAttributePoints() const
{
	return CachedPlayerState->CharacterContextComponent->GetAttributePoints();
}

int32 AParasiteCharacter::GetLevel()
{
	return CachedPlayerState->CharacterContextComponent->GetLevel();
}

EGrowthRate AParasiteCharacter::GetGrowthRate() const
{
	return CachedPlayerState->CharacterContextComponent->GetGrowthRate();
}

int32 AParasiteCharacter::GetBaseXP() const
{
	return CachedPlayerState->CharacterContextComponent->GetBaseXP();
}

float AParasiteCharacter::GetXPMultiplierAmount()
{
	// TODO: Adjust based on current Multiplier Bonuses
	return 1;
}

UAbilitySystemComponent* AParasiteCharacter::GetAbilitySystemComponent() const
{
	return GetPlayerState<AParasitePlayerState>()->GetAbilitySystemComponent();
};

bool AParasiteCharacter::GetIsInhabited() const
{
	return false; // Will never be inhabited
}
