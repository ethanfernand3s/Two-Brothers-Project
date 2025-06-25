#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/BaseGameplayAbility.h"
#include "AbilitySystem/BaseGameplayAbility_ClientToServer.h"
#include "Characters/BaseAnimalCharacter.h"
#include "PossessHostAbility.generated.h"

class UEnterHuntingModeAbility;
class ABaseAnimalCharacter;
class UPossessMiniGameUserWidget;

/**
* Launches the parasite, runs the 10-s spam mini-game on the owner client,
* then either possesses the target or ejects with FX.
*/
UCLASS()
class TWOBROTHERSPROJECT_API UPossessHostAbility : public UBaseGameplayAbility_ClientToServer
{
    GENERATED_BODY()

public:
    UPossessHostAbility();
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    
    //~UBaseGameplayAbility_ClientToServer interface
   virtual void ActivateLocalPlayerAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
       const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void ActivateAbilityWithTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle,
        FGameplayTag ApplicationTag) override;

private:

    virtual void EndAbilityCleanup(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    //~End of UBaseGameplayAbility_ClientToServer interface
    
    void LaunchParasitePawn(const FVector& LaunchDirection);

    UFUNCTION()
    void OnActorHit(
        AActor*                 SelfActor,
        AActor*                 OtherActor,
        FVector                 NormalImpulse,
        const FHitResult&       Hit);

    UFUNCTION()
    void HandleMiniGameStart(FGameplayEventData EventData);
    
    UFUNCTION()
    void OnMiniGameWidgetFinished(bool bWidgetWon, float NormalizedTimeLeft);
    
    void FinishMiniGame(bool bBarWon, float NormalizedTimeLeft);

    void TriggerEjectionFX(const FGameplayTag& SocketTag);

    void EndSeekHostAbility();

    UPROPERTY(EditDefaultsOnly, Category = "Possess|Launch")
    float LaunchSpeed = 3800.f;

    UPROPERTY(EditDefaultsOnly, Category = "Possess|HitCheck")
    float SocketRange = 120.f;

    UPROPERTY(EditDefaultsOnly, Category = "Possess|FX")
    float PulseDuration = 0.35f;
    UPROPERTY(EditDefaultsOnly, Category = "Possess|FX")
    float CaptureDelay = 1.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Possess|Classes")
    TSubclassOf<UPossessMiniGameUserWidget> MiniGameWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "Possess|Classes")
    TSubclassOf<UEnterHuntingModeAbility> SeekHostClass;
    
    UPROPERTY()
    TWeakObjectPtr<ABaseAnimalCharacter> TargetAnimalPtr;

    FPossessionSocketData CachedSocketData;
    bool bFinishHandled = false;
};
