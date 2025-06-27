// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Parasite/Abilities/Possessing/UnPossessHostAbility.h"

#include "Characters/ParasiteCharacter.h"

UUnPossessHostAbility::UUnPossessHostAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
}

void UUnPossessHostAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) return;

	APlayerController* PC = Cast<APlayerController>(ActorInfo->PlayerController);
	if (!PC || !ParasiteCharacterClass) { EndAbility(Handle, ActorInfo, ActivationInfo, true, false); return; }


	FVector  SpawnLoc = PC->GetPawn()->GetActorLocation() + FVector(0.f, 0.f, 20.f);
	FRotator SpawnRot = PC->GetPawn()->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner                          = PC;                    
	SpawnParams.Instigator                     = PC->GetPawn();          
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// TODO: Need better solution then spawning 20 units above the head cuz this could cause the parasite to spawn above in something colliding
	AParasiteCharacter* NewParasite =
		GetWorld()->SpawnActor<AParasiteCharacter>(
			ParasiteCharacterClass,           
			SpawnLoc,
			SpawnRot,
			SpawnParams);                    
	PC->Possess(NewParasite);
			
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
