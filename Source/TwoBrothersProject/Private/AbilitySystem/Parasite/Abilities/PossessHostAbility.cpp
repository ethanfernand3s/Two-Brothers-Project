

#include "AbilitySystem/Parasite/Abilities/PossessHostAbility.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "TBGameplayTags.h"
#include "AbilitySystem/Interfaces/IPossessable.h"
#include "Characters/BaseAnimalCharacter.h"
#include "MiniGames/Possess/PossessMiniGame.h"
#include "UI/HUD/PlayerHUD.h"


UPossessHostAbility::UPossessHostAbility()
{
   InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
   NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
   ActivationRequiredTags.AddTag(FTBGameplayTags::Get().State_Parasite_SeekingHost);
   
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
   PerformPossessionTrace();
   
   EndAbility(Handle, ActorInfo, ActivationInfo, /*bReplicateEnd=*/false, /*bWasCancelled=*/false);
}

void UPossessHostAbility::PerformPossessionTrace()
{
   APlayerController* PC = Cast<APlayerController>(GetCurrentActorInfo()->PlayerController);
   FVector  Start;
   FRotator ViewRot;
   PC->GetPlayerViewPoint(Start, ViewRot);            // replicated control rotation
   const FVector End = Start + ViewRot.Vector() * SocketHitRange;


   // Line‑trace and attempt possession
   FHitResult Hit; 
   FCollisionQueryParams Params{TEXT("PossessTrace"), /*bTraceComplex=*/false, GetCurrentActorInfo()->AvatarActor.Get()};
   if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params))
   {
      if (AActor* HitActor = Hit.GetActor())
      {
         {
            // PossessMiniGameResult(ActorInfo);
            if (ABaseAnimalCharacter* Animal = Cast<ABaseAnimalCharacter>(HitActor))
            {
               FName ClosestSocket = Animal->FindClosestPossessionSocket(Hit.ImpactPoint);
               float Dist = FVector::Dist(Animal->GetSocketLocation(ClosestSocket), Hit.ImpactPoint);

               if (Dist <= SocketHitRange && Animal->CanBePossessedAtSocket(ClosestSocket))
               {
                  StartMiniGame(Animal, ClosestSocket);
               }
            }
         }
      }
   }
}

void UPossessHostAbility::StartMiniGame(AActor* TargetAnimal, FName Socket)
{ if (!GetWorld() || !PossessMiniGameClass) return;

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
   if (APlayerController* PC = Cast<APlayerController>(GetCurrentActorInfo()->PlayerController))
   if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PC->GetHUD()))
   {
      PlayerHUD->PossessMiniGame();
   }
   
   // On finish → call OnMiniGameCompleted with result
}

void UPossessHostAbility::OnMiniGameCompleted(bool bSuccess, float ExecutionBonus)
{
   //float FinalChance = /* Calculate from socket base chance + ExecutionBonus */;
   // Feed FinalChance into LuckyWidget
   // On Widget Success: Trigger Server_PossessAnimal()
   EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

// UExecutePossessionAbility (ServerOnly)
// Calls Possess(TargetAnimal);
