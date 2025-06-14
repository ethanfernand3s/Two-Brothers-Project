

#include "AbilitySystem/Parasite/Abilities/PossessHostAbility.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Interfaces/IPossessable.h"
#include "MiniGames/Possess/PossessMiniGame.h"
#include "UI/HUD/PlayerHUD.h"


UPossessHostAbility::UPossessHostAbility()
{
   InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
   NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
   LineTraceLength = 200.f;
   
}


void UPossessHostAbility::ActivateAbility
(
      const FGameplayAbilitySpecHandle Handle,
      const FGameplayAbilityActorInfo* ActorInfo,
      const FGameplayAbilityActivationInfo ActivationInfo,
      const FGameplayEventData* TriggerEventData)
{
   // Only continue if we have authority (dedicated or listen server).
   if (!ActorInfo->IsLocallyControlled())
   {
      EndAbility(Handle, ActorInfo, ActivationInfo, /*bReplicateEnd=*/false, /*bWasCancelled=*/true);
      return;
   }
   
   APlayerController* PC = Cast<APlayerController>(ActorInfo->PlayerController);
   if (!PC )
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
            PossessMiniGameResult(ActorInfo);
          
           
         }
      }
   }


   EndAbility(Handle, ActorInfo, ActivationInfo, /*bReplicateEnd=*/false, /*bWasCancelled=*/false);
}

void UPossessHostAbility::OnPossessResultReceived(bool bSuccess)
{
   Server_OnPossessResultReceived_Implementation(bSuccess);
}

void UPossessHostAbility::Server_OnPossessResultReceived_Implementation(bool bSuccess)
{
   
}

void UPossessHostAbility::PossessMiniGameResult(const FGameplayAbilityActorInfo* ActorInfo)
{
   if (!GetWorld() || !PossessMiniGameClass) return;

   FVector SpawnLocation = FVector(0.f, 0.f, -1000.f);
   FRotator SpawnRotation = FRotator::ZeroRotator;
   FTransform SpawnTransform(SpawnRotation, SpawnLocation);

   FActorSpawnParameters SpawnParams;
   SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

   APossessMiniGame* MiniGame = GetWorld()->SpawnActor<APossessMiniGame>(
      PossessMiniGameClass, 
      SpawnTransform, 
      SpawnParams
   );

   if (MiniGame)
   {
      UE_LOG(LogTemp, Log, TEXT("Spawned PossessMiniGame BP successfully"));
   }
   else
   {
      UE_LOG(LogTemp, Warning, TEXT("Failed to spawn PossessMiniGame"));
   }
   APlayerController* PC = Cast<APlayerController>(ActorInfo->PlayerController);
   if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PC->GetHUD()))
   {
      PlayerHUD->PossessMiniGame();
   }
}
