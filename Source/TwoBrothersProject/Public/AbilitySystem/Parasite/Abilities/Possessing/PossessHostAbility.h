#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Characters/BaseAnimalCharacter.h"
#include "GameplayCueManager.h" 
#include "AbilitySystem/Base/Abilities/BaseGameplayAbility_ClientToServer.h"
#include "PossessHostAbility.generated.h"


class URarityDataAsset;
enum class ERarity : uint8;
class AParasiteCharacter;
class USeekHostAbility;
class ABaseAnimalCharacter;
class UPossessMiniGameUserWidget;
struct FActiveGameplayCueHandle;
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
    
    //~UGameplayAbility interface
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    //~End of UGameplayAbility interface
    
    //~UBaseGameplayAbility_ClientToServer interface
   virtual void ActivateLocalPlayerAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
       const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void ActivateAbilityWithTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle,
        FGameplayTag ApplicationTag) override;
    
private:

    virtual void EndAbilityCleanup(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    //~End of UBaseGameplayAbility_ClientToServer interface

#pragma region Camera Functions

    void TransferParasitePostProcessToAnimal(bool bApply);
    
#pragma endregion Camera Functions
    
#pragma region Launching Functions
    
    void LaunchParasitePawn(FVector LaunchDirection);

    UFUNCTION()
    void OnActorHit(
        AActor*                 SelfActor,
        AActor*                 OtherActor,
        FVector                 NormalImpulse,
        const FHitResult&       Hit);

#pragma endregion Launching Functions

#pragma region MiniGame Functions
    
    void StartBurrowCue();
    
    void SendBurrowDepthUpdate(float NewDepth);
    void EndBurrowCue();
    void ReceiveNewDepthFromClient(float NewDepth);
    void NotifyServerToPlayBurrowAnim();
    void PlayBurrowAnim();
    UFUNCTION()
    void HandleMiniGameStart(FGameplayEventData EventData);
    UFUNCTION()
    void OnMiniGameWidgetFinished(bool bWidgetWon, float NormalizedTimeLeft);
    void FinishMiniGame(bool bBarWon, float NormalizedTimeLeft);
    
#pragma endregion MiniGame Functions

#pragma region Finishing Functions
    
    void TriggerEjectionFX(const FGameplayTag& SocketTag);

    void EndSeekHostAbility();

#pragma endregion Finishing Functions

#pragma region SubClassess
    
    UPROPERTY(EditDefaultsOnly, Category = "Possess|Classes")
    TSubclassOf<UPossessMiniGameUserWidget> MiniGameWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "Possess|Classes")
    TSubclassOf<USeekHostAbility> SeekHostClass;

#pragma endregion SubClassess

#pragma region Montages
    
    UPROPERTY(EditDefaultsOnly, Category = "Possess|Anim", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> PossessSuccessMontage;
    UPROPERTY(EditDefaultsOnly, Category = "Possess|Anim", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> BurrowInMontage;

#pragma endregion Montages
    
#pragma region Launching Values
    
    UPROPERTY(EditDefaultsOnly, Category = "Possess|Launch", meta = (AllowPrivateAccess))
    float LaunchSpeed = 1500.f;

    UPROPERTY(EditDefaultsOnly, Category = "Possess|HitCheck", meta = (AllowPrivateAccess))
    float SocketRange = 100.f;

#pragma endregion Launching Values

#pragma region MiniGame Values
    
    UPROPERTY(EditDefaultsOnly, Category = "Possess|Minigame|UI")
    float PulseDuration = 0.35f;
    UPROPERTY(EditDefaultsOnly, Category = "Possess|Minigame|UI")
    float CaptureDelay = 1.0f;
    
#pragma endregion MiniGame Values

#pragma region Cached Values

    UPROPERTY()
    TWeakObjectPtr<ABaseAnimalCharacter> TargetAnimalPtr;
    
    float CachedAnimalCombatPower = 0.f;
    
    FGameplayTag CachedAnimalRarity = FGameplayTag::EmptyTag;

    FPossessionSocketData CachedAnimalSocketData;
    
    FVector CachedBurrowStartLocation = FVector::ZeroVector;
    FVector CachedHoleNormal = FVector::UpVector;

    FPostProcessSettings SavedAnimalPP;
    float                SavedAnimalPPWeight = 0.f;
    bool                 bAnimalPPSaved     = false;

#pragma endregion Cached Value 

    bool bIsPossessFinished = false;
    bool bHasBurrowEnded = false;
};
