// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Parasite/Abilities/PossessHostAbility.h"

#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Interfaces/IPossessable.h"

UPossessHostAbility::UPossessHostAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	LineTraceLength = 200.f;
}

void UPossessHostAbility::ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)
{
	// Only continue if we have authority (dedicated or listen server).
	if (!ActorInfo || !ActorInfo->IsNetAuthority() || !CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, /*bReplicateEnd=*/false, /*bWasCancelled=*/false);
		return;
	}

	// Get server‑side controller and pawn
	APlayerController* PC = Cast<APlayerController>(ActorInfo->PlayerController);
	APawn*              Avatar = Cast<APawn>(ActorInfo->AvatarActor.Get());
	if (!PC || !Avatar)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Use a view that EXISTS on the server
	FVector  Start;
	FRotator ViewRot;
	PC->GetPlayerViewPoint(Start, ViewRot);            // replicated control rotation
	const FVector End = Start + ViewRot.Vector() * LineTraceLength;

	// Line‑trace and attempt possession
	FHitResult Hit;
	FCollisionQueryParams Params{TEXT("PossessTrace"), /*bTraceComplex=*/false, Avatar};
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params))
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<UPossessable>() &&
				Cast<IPossessable>(HitActor)->CanBePossessedBy())
			{
				// Destroy current pawn, then possess the new pawn (server‑side)
				Avatar->Destroy();
				PC->Possess(Cast<APawn>(HitActor));
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, /*bReplicateEnd=*/false, /*bWasCancelled=*/false);
}