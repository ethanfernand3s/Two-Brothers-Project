// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Base/Abilities/BaseProjectileAbility.h"


#include "Actor/ProjectileActor.h"
#include "Characters/BaseCharacter.h"


void UBaseProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		const FVector SocketLocation = Character->GetProjectileCombatSocketLocation();
	
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		//TODO: Set the projectileRotation 
		AProjectileActor* Projectile = GetWorld()->SpawnActorDeferred<AProjectileActor>(
			ProjectileActor
			,SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		//TODO: Give the Projectile A Gameplay Effect Spec for causing Damage.
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
