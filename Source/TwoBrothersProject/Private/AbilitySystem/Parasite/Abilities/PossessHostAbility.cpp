#include "AbilitySystem/Parasite/Abilities/Possessing/PossessHostAbility.h"
#include "UI/Widget/Possession/PossessMiniGameUserWidget.h"
#include "TBGameplayTags.h"
#include "Characters/BaseAnimalCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Animal/AnimalAttributeSet.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"
#include "AbilitySystem/Parasite/Abilities/Possessing/SeekHostAbility.h"
#include "AbilitySystem/TargetData/PossessResultTargetData.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "Characters/CharacterContextComponent.h"
#include "GameFramework/PlayerState.h"
#include "Player/ParasitePlayerState.h"

using FTBTags = FTBGameplayTags;

/* ctor */
/* ------------------------------------------------------------------------ */
UPossessHostAbility::UPossessHostAbility()
{
    InstancingPolicy   = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
    
    ActivationRequiredTags.AddTag(FTBTags::Get().State_Parasite_SeekingHost);
}

void UPossessHostAbility::ActivateAbility(
        const FGameplayAbilitySpecHandle            Handle,
        const FGameplayAbilityActorInfo*            Info,
        const FGameplayAbilityActivationInfo        ActInfo,
        const FGameplayEventData* Trigger)
{

    if (!CommitAbility(Handle, Info, ActInfo))
    {
        EndAbility(Handle, Info, ActInfo, true, true);
        return;
    }
    
    const APlayerController* PC = Cast<APlayerController>(Info->PlayerController);

    FVector  EyeLoc;
    FRotator EyeRot;
    PC->GetPlayerViewPoint(EyeLoc, EyeRot);

    LaunchParasitePawn(EyeRot.Vector());

    ACharacter* Pawn = Cast<ACharacter>(Info->AvatarActor);
    Pawn->OnActorHit.AddUniqueDynamic(this, &UPossessHostAbility::OnActorHit);

    /* continue with base dispatch (Local vs Server) */
    Super::ActivateAbility(Handle, Info, ActInfo, Trigger);
}

void UPossessHostAbility::ActivateLocalPlayerAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    UAbilityTask_WaitGameplayEvent* WaitStart =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
            this,
            FTBTags::Get().Event_MiniGame_Start);

    WaitStart->EventReceived.AddDynamic(
        this,
        &UPossessHostAbility::HandleMiniGameStart);

    WaitStart->ReadyForActivation();
}

void UPossessHostAbility::LaunchParasitePawn(const FVector& Dir)
{
    ACharacter* Pawn = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    Pawn->LaunchCharacter(Dir * LaunchSpeed, true, true);
}

void UPossessHostAbility::OnActorHit(
        AActor*           Self,
        AActor*           Other,
        FVector,
        const FHitResult& Hit)
{
    Self->OnActorHit.RemoveDynamic(this, &UPossessHostAbility::OnActorHit);

    TargetAnimalPtr = Cast<ABaseAnimalCharacter>(Other);
    if (!TargetAnimalPtr.IsValid())
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
        return;
    }

    FPossessionSocketData SocketData =
        TargetAnimalPtr->FindClosestPossessionSocket(Hit.ImpactPoint);

    float Dist =
        FVector::Dist(
            TargetAnimalPtr->GetCurrentSocketLocation(SocketData.SocketGameplayTag),
            Hit.ImpactPoint);

    bool bBadRange   = Dist > SocketRange;
    bool bNoChance   = SocketData.PossessionChance == 0.f;
    bool bHasOwner   = TargetAnimalPtr->GetOwner() != nullptr;
    
    if (bBadRange || bNoChance || bHasOwner)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
        return;
    }

    CachedSocketData = SocketData;
    CachedAnimalCombatPower = TargetAnimalPtr->GetAttributeSet()->CalculateCombatPower();
    CachedAnimalAuraColor = TargetAnimalPtr->CharacterContextComponent->GetAuraColor();

    // Send start minigame to client
    FGameplayEventData StartMiniGameEvent;
    StartMiniGameEvent.EventTag       = FTBTags::Get().Event_MiniGame_Start;
    StartMiniGameEvent.EventMagnitude = SocketData.PossessionChance;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetOwningActorFromActorInfo(),
        StartMiniGameEvent.EventTag,
        StartMiniGameEvent);
}

void UPossessHostAbility::HandleMiniGameStart(FGameplayEventData /*Unused*/)
{
    checkf(MiniGameWidgetClass, TEXT("Invalid! Must set MiniGameWidgetClass in BP."));

    UPossessMiniGameUserWidget* W =
        CreateWidget<UPossessMiniGameUserWidget>(GetWorld(), MiniGameWidgetClass);

    if (!W) return;

    const AParasitePlayerState* PlayerState = Cast<AParasitePlayerState>(GetOwningActorFromActorInfo());
    if (PlayerState == nullptr)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
        return;
    }
    
    const float ParasiteCombatPower = PlayerState->GetParasiteAttributeSet()->CalculateCombatPower();

    const float Scale = 0.0025f;
    const float Base  = 0.04f;

    const float TapInc = FMath::Clamp(Base + (ParasiteCombatPower - CachedAnimalCombatPower) * Scale, 0.01f, 0.07f);
    const float EnemyPerSec = FMath::Clamp(Base + (CachedAnimalCombatPower - ParasiteCombatPower) * Scale, 0.01f, 0.07f);

    W->Init(CachedSocketData.PossessionChance, TapInc, EnemyPerSec, PlayerState->CharacterContextComponent->GetAuraColor(), CachedAnimalAuraColor);
    W->OnFinished.AddDynamic(this, &UPossessHostAbility::OnMiniGameWidgetFinished);
    W->AddToViewport();

    FInputModeUIOnly Mode;
    Mode.SetWidgetToFocus(W->TakeWidget());
    GetActorInfo().PlayerController->SetInputMode(Mode);
    GetActorInfo().PlayerController->SetShowMouseCursor(true);
}

void UPossessHostAbility::OnMiniGameWidgetFinished(bool bWidgetWon, float NormalizedTimeLeft)
{
    if (!IsLocallyControlled()) return;
    
    FGameplayAbilityTargetDataHandle TargetDataHandle;
    auto* TargetData = new FGameplayAbilityTargetData_PossessResult();
    TargetData->bWon         = bWidgetWon;
    TargetData->NormalizedTimeLeft = NormalizedTimeLeft;
    TargetDataHandle.Add(TargetData);

    NotifyTargetDataReady(TargetDataHandle, FGameplayTag());
}

void UPossessHostAbility::ActivateAbilityWithTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle,
    FGameplayTag ApplicationTag)
{
    if (bFinishHandled) return;
    bFinishHandled = true;
    
    auto* PossessResultTargetData =
        static_cast<const FGameplayAbilityTargetData_PossessResult*>(TargetDataHandle.Get(0));
    
    bool  bBarWon        = PossessResultTargetData->bWon;
    float NormalizedLeft = PossessResultTargetData->NormalizedTimeLeft;
    
    FinishMiniGame(bBarWon, NormalizedLeft);
}

/* FinishMiniGame */
void UPossessHostAbility::FinishMiniGame(bool bBarWon, float NormalizedTimeLeft)
{
    bool bSuccess = false;

    float FinalChance = CachedSocketData.PossessionChance;

    if (bBarWon)
    {
        float Bonus = FMath::Clamp(NormalizedTimeLeft / 10.0f, 0.0f, 0.1f);
        FinalChance = FMath::Clamp(FinalChance + Bonus, 0.0f, 1.0f);
        bSuccess = FMath::FRand() < FinalChance;
    }
    else
    {
        TriggerEjectionFX(CachedSocketData.SocketGameplayTag);
        return;
    }

    int32 PulseCount = 0;
    if (bSuccess)                    PulseCount = 3;
    else if (FinalChance >= 0.66f)   PulseCount = 2;
    else                             PulseCount = 1;

    for (int32 Index = 0; Index < PulseCount; ++Index)
    {
        float Delay = Index * PulseDuration;

        FTimerHandle PulseHandle;
        GetWorld()->GetTimerManager().SetTimer(
            PulseHandle,
            [ASC = GetAbilitySystemComponentFromActorInfo(),
             PulseCue = FTBTags::Get().GameplayCue_Possession_Pulse,
             Animal   = TargetAnimalPtr]()
            {
                FGameplayCueParameters Params;
                Params.SourceObject = Animal.Get();
                Params.RawMagnitude = 1.f;
                ASC->ExecuteGameplayCue(PulseCue, Params);
            },
            Delay,
            false);
    }

    float ResolveDelay = PulseCount * PulseDuration + CaptureDelay;

    FTimerHandle FinishHandle;
    GetWorld()->GetTimerManager().SetTimer(
        FinishHandle,
        [this,
         bSuccess,
         ASC          = GetAbilitySystemComponentFromActorInfo(),
         Animal       = TargetAnimalPtr,
         SuccessCue   = FTBTags::Get().GameplayCue_Possession_Captured,
         FailCue      = FTBTags::Get().GameplayCue_Possession_Fail]()
        {
            FGameplayCueParameters Params;
            Params.SourceObject = Animal.Get();
            Params.RawMagnitude = 1.f;
            ASC->ExecuteGameplayCue(bSuccess ? SuccessCue : FailCue, Params);

            if (bSuccess && Animal.IsValid())
            {
                const auto ActInfo = GetCurrentActivationInfo();
                if (HasAuthority(&ActInfo))
                {
                    // Since inhabit succeeded we destroy parasite actor
                    GetActorInfo().AvatarActor->Destroy();

                    // Then inhabit
                    AController* PC = GetActorInfo().PlayerController.Get();
                    PC->Possess(Animal.Get());
                }

                
                EndAbility(CurrentSpecHandle,
                       CurrentActorInfo,
                       CurrentActivationInfo,
                       false,
                       false);
            }
            else
            {
                TriggerEjectionFX(CachedSocketData.SocketGameplayTag);
            }
        },
        ResolveDelay,
        false);
}

/* TriggerEjectionFX */
void UPossessHostAbility::TriggerEjectionFX(const FGameplayTag& SocketTag)
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

    const FGameplayTag PoopCue   = FTBTags::Get().GameplayCue_Possession_Eject_Poop;
    const FGameplayTag VomitCue  = FTBTags::Get().GameplayCue_Possession_Eject_Vomit;

    bool bButtSocket = SocketTag == FTBTags::Get().Socket_Butt;

    FGameplayCueParameters Params;
    Params.SourceObject = TargetAnimalPtr.Get();
    Params.Location     = TargetAnimalPtr->GetCurrentSocketLocation(SocketTag);

    ASC->ExecuteGameplayCue(bButtSocket ? PoopCue : VomitCue, Params);

    ACharacter* Parasite = Cast<ACharacter>(GetAvatarActorFromActorInfo());

    FVector ImpulseDir = bButtSocket ?
        -TargetAnimalPtr->GetActorForwardVector() :
         TargetAnimalPtr->GetActorForwardVector();

    Parasite->LaunchCharacter(ImpulseDir * 600.f, true, true);

    EndAbility(CurrentSpecHandle,
               CurrentActorInfo,
               CurrentActivationInfo,
               false,
               false);
}

void UPossessHostAbility::EndSeekHostAbility()
{
    checkf(SeekHostClass, TEXT("Invalid! Must set SeekHostClass in BP."));
    if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
    {
        // Open a prediction window if we are on the client
        FScopedPredictionWindow PredWin(ASC,IsPredictingClient());

        // Cancel ability with class default object
        UGameplayAbility* SeekHostCDO = SeekHostClass->GetDefaultObject<UGameplayAbility>();
        ASC->CancelAbility(SeekHostCDO);
    }
}

void UPossessHostAbility::EndAbilityCleanup(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            bool bReplicateEndAbility, bool bWasCancelled)
{
    EndSeekHostAbility();
    
    // Set input back to game
    ActorInfo->PlayerController->SetInputMode(FInputModeGameOnly());
    ActorInfo->PlayerController->SetShowMouseCursor(false);
    
    Super::EndAbilityCleanup(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
