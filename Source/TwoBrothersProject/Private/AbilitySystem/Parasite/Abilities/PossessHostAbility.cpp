// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Parasite/Abilities/PossessHostAbility.h"

#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Interfaces/IPossessable.h"

UPossessHostAbility::UPossessHostAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	LineTraceLength = 200.f;
}

void UPossessHostAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
									  const FGameplayAbilityActorInfo* ActorInfo,
									  const FGameplayAbilityActivationInfo ActivationInfo,
									  const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) return;

	APlayerController* PC = Cast<APlayerController>(ActorInfo->PlayerController);
	if (!PC) { EndAbility(Handle, ActorInfo, ActivationInfo, true, false); return; }
	
	FVector  Start = PC->PlayerCameraManager->GetCameraLocation();
	FVector  Dir   = PC->PlayerCameraManager->GetCameraRotation().Vector(); // or use ControlRotation
	FVector  End   = Start + Dir * LineTraceLength;
	FHitResult Hit;
	FCollisionQueryParams Params(TEXT("PossessTrace"), false, PC->GetPawn());
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params))
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<UPossessable>() && Cast<IPossessable>(HitActor)->CanBePossessedBy() > 0)
			{
				// TODO: Add Something chance/mini game based on factors in CanBePossessedBy
				PC->GetPawn()->Destroy();
				PC->Possess(Cast<APawn>(HitActor));
			}
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
