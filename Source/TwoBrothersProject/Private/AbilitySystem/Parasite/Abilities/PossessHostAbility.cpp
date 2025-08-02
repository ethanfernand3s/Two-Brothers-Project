#include "AbilitySystem/Parasite/Abilities/Possessing/PossessHostAbility.h"
#include "UI/Widget/Possession/PossessMiniGameUserWidget.h"
#include "TBGameplayTags.h"
#include "Characters/BaseAnimalCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Animal/AnimalAttributeSet.h"
#include "AbilitySystem/Parasite/ParasiteAttributeSet.h"
#include "AbilitySystem/Parasite/Abilities/Possessing/SeekHostAbility.h"
#include "AbilitySystem/TargetData/BurrowDepthUpdateTargetData.h"
#include "AbilitySystem/TargetData/PossessResultTargetData.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Characters/CharacterContextComponent.h"
#include "Characters/ParasiteCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Player/ParasitePlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"

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

    Super::ActivateAbility(Handle, Info, ActInfo, Trigger);
    
    const APlayerController* PC = Cast<APlayerController>(Info->PlayerController);
    ACharacter* Pawn = Cast<ACharacter>(Info->AvatarActor);
    Pawn->OnActorHit.AddUniqueDynamic(this, &UPossessHostAbility::OnActorHit);
    
    FVector  EyeLoc;
    FRotator EyeRot;
    PC->GetPlayerViewPoint(EyeLoc, EyeRot);

    LaunchParasitePawn(EyeRot.Vector());
}

void UPossessHostAbility::ActivateLocalPlayerAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    UAbilityTask_WaitGameplayEvent* WaitStart =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
            this,
            FTBTags::Get().Event_MiniGame_Start);

    if (WaitStart)
    {
        WaitStart->EventReceived.AddDynamic(
       this,
       &UPossessHostAbility::HandleMiniGameStart);

        WaitStart->Activate();
    }
}

void UPossessHostAbility::LaunchParasitePawn(FVector CamDir)
{
    ACharacter* Parasite = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    if (!Parasite) return;
    
    CamDir.Z = 0.f;                     // flatten pitch
    CamDir   = CamDir.GetSafeNormal();  // ensure unit length

    constexpr float VerticalBoost = 600.f;   
    FVector LaunchVel = CamDir * LaunchSpeed + FVector(0.f, 0.f, VerticalBoost);
    
    Parasite->LaunchCharacter(LaunchVel,
                              /*bXYOverride=*/true,
                              /*bZOverride=*/true);
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

    // Cache Values
    CachedAnimalSocketData = SocketData;
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

    // 1) Freeze parasite movement
    AttachParasiteToAnimal();  

    // 2) Start the cue
    StartBurrowCue();

    // 3) Copy PP now; still looking through parasite cam
    TransferParasitePostProcessToAnimal(true);

    // 4) Finally blend the view target
    APlayerController* PC = GetActorInfo().PlayerController.Get();
    if (PC)
    {
        FViewTargetTransitionParams Blend;
        Blend.BlendTime     = 0.25f;
        Blend.BlendFunction = VTBlend_Cubic;
        PC->SetViewTarget(TargetAnimalPtr.Get(), Blend);
    }
}

void UPossessHostAbility::TransferParasitePostProcessToAnimal(bool bApply)
{
    if (!TargetAnimalPtr.IsValid()) return;

    // 1) Get parasite & animal camera components
    APlayerController* PC = GetActorInfo().PlayerController.Get();
    if (!PC || !PC->GetPawn()) return;

    UCameraComponent* ParasiteCam = PC->GetPawn()
                                      ->FindComponentByClass<UCameraComponent>();
    UCameraComponent* AnimalCam   = TargetAnimalPtr->FindComponentByClass<UCameraComponent>();
    if (!ParasiteCam || !AnimalCam) return;

    if (bApply)
    {
        /* ---------- SAVE ORIGINAL ONCE ---------- */
        if (!bAnimalPPSaved)
        {
            SavedAnimalPP        = AnimalCam->PostProcessSettings;
            SavedAnimalPPWeight  = AnimalCam->PostProcessBlendWeight;
            bAnimalPPSaved       = true;
            
        }

        /* ---------- COPY PARASITE -> ANIMAL ---------- */
        AnimalCam->PostProcessSettings     = ParasiteCam->PostProcessSettings;
        AnimalCam->PostProcessBlendWeight  = ParasiteCam->PostProcessBlendWeight; // often 1.0
    }
    else if (bAnimalPPSaved)
    {
        /* ---------- RESTORE ORIGINAL ---------- */
        AnimalCam->PostProcessSettings     = SavedAnimalPP;
        AnimalCam->PostProcessBlendWeight  = SavedAnimalPPWeight;
        bAnimalPPSaved = false;                        // reset cache
    }
}

void UPossessHostAbility::AttachParasiteToAnimal()
{
    if (AParasiteCharacter* ParasiteChar = Cast<AParasiteCharacter>(GetAvatarActorFromActorInfo()))
    {
        if (GetActorInfo().IsNetAuthority())
        {
            ParasiteChar->GetCharacterMovement()->DisableMovement();
                                                
            // TODO: Doesn't always start at 50% make it match with cached socket percentage

            // Multicast function because collision isn't auto replicated
            if (TargetAnimalPtr.IsValid() && CachedAnimalSocketData.SocketGameplayTag.IsValid())
            {
                ParasiteChar->SetBurrowCollision(false, TargetAnimalPtr.Get());
                
                USkeletalMeshComponent* AnimalMeshComponent = TargetAnimalPtr->GetMesh();
                FName RequestedSocketName = CachedAnimalSocketData.SocketGameplayTag.GetTagName();
                // Attach to animal
                if (AnimalMeshComponent)
                {
                    ParasiteChar->AttachToComponent(
                            AnimalMeshComponent,
                            FAttachmentTransformRules::SnapToTargetNotIncludingScale,
                            RequestedSocketName);
                }

                ParasiteChar->ForceNetUpdate();
            }
        }
    }
}


/*  Spawn once — called from OnActorHit after you cache start vectors */
void UPossessHostAbility::StartBurrowCue()
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (!ASC) return;

    FGameplayCueParameters P;
    P.Instigator   = GetAvatarActorFromActorInfo();
    P.SourceObject = this;
    P.Location     = CachedBurrowStartLocation;   // start pos
    P.Normal       = CachedHoleNormal;            // outward normal
    P.RawMagnitude = 0.f;                         // depth gauge

    ASC->AddGameplayCue(FTBGameplayTags::Get().GameplayCue_Possession_BurrowingIn, P);
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

    W->Init(CachedAnimalSocketData.PossessionChance, TapInc, EnemyPerSec, PlayerState->CharacterContextComponent->GetAuraColor(), CachedAnimalAuraColor);
    W->OnFinished.AddDynamic(this, &UPossessHostAbility::OnMiniGameWidgetFinished);
    W->OnGaugeChanged.BindUObject(this, &UPossessHostAbility::ReceiveNewDepthFromClient);
    W->OnSpaceBarPressed.BindUObject(this, &UPossessHostAbility::NotifyServerToPlayBurrowAnim);
    W->AddToViewport();

    FInputModeUIOnly Mode;
    Mode.SetWidgetToFocus(W->TakeWidget());
    GetActorInfo().PlayerController->SetInputMode(Mode);
    GetActorInfo().PlayerController->SetShowMouseCursor(false);
}

// Packages up minigame result target data from client -> server
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

// Packages up updated burrow depth target data from client -> server
void UPossessHostAbility::ReceiveNewDepthFromClient(float InNewDepth)
{
    if (!IsLocallyControlled()) return;

    FGameplayAbilityTargetDataHandle TargetDataHandle;
    auto* TargetData = new FGameplayAbilityTargetData_BurrowDepthUpdate();
    TargetData->NewDepth = InNewDepth;
    TargetDataHandle.Add(TargetData);
    
    NotifyTargetDataReady(TargetDataHandle, FGameplayTag());
}

void UPossessHostAbility::NotifyServerToPlayBurrowAnim()
{
    if (!IsLocallyControlled()) return;

    FGameplayAbilityTargetDataHandle Handle;
    Handle.Add(new FGameplayAbilityTargetData_LocationInfo());
    
    // Pass both as empty since this delegate doesn't carry info
    NotifyTargetDataReady(Handle, FGameplayTag());
}

void UPossessHostAbility::PlayBurrowAnim()
{
    if (BurrowInMontage)
    {
        UAbilityTask_PlayMontageAndWait* Task =
            UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
                this,
                FName("BurrowIn"),      // task instance name
                BurrowInMontage,
                1.f,                    // rate
                NAME_None,              // section
                false);                 // stop on ability end
        Task->Activate();               // handles prediction + replication
    }
}

// Receives package from client and extracts target data
void UPossessHostAbility::ActivateAbilityWithTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle,
    FGameplayTag ApplicationTag)
{
    const FGameplayAbilityTargetData* Raw = TargetDataHandle.Get(0);
    
    
    if (!Raw) return;

    /* Possess result? */
    if (Raw->GetScriptStruct() ==
        FGameplayAbilityTargetData_PossessResult::StaticStruct())
    {
        const auto* Possess = static_cast<
            const FGameplayAbilityTargetData_PossessResult*>(Raw);

        FinishMiniGame(Possess->bWon, Possess->NormalizedTimeLeft);
    }
    /* Burrow depth update? */
    else if (Raw->GetScriptStruct() ==
             FGameplayAbilityTargetData_BurrowDepthUpdate::StaticStruct())
    {
        const auto* Depth = static_cast<
            const FGameplayAbilityTargetData_BurrowDepthUpdate*>(Raw);

        SendBurrowDepthUpdate(Depth->NewDepth);
    }
    /* User pressed Space to burrow | TLDR using LocationInfo cuz I don't want to make another target data for nothing */
    else if (Raw->GetScriptStruct() == FGameplayAbilityTargetData_LocationInfo::StaticStruct())
    {
        PlayBurrowAnim();
    }
}

/*  Executes Burrow Update Cue SERVER ONLY */
void UPossessHostAbility::SendBurrowDepthUpdate(float NewDepth)
{ 
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (!ASC) return;

    FGameplayCueParameters P;
    P.RawMagnitude = FMath::Clamp(NewDepth, 0.f, 1.f);

    ASC->ExecuteGameplayCue(
       FTBGameplayTags::Get().GameplayCue_Possession_BurrowingIn, P);
}

/* FinishMiniGame SERVER ONLY */
void UPossessHostAbility::FinishMiniGame(bool bBarWon, float NormalizedTimeLeft)
{
    bool bSuccess = false;
    float FinalChance = CachedAnimalSocketData.PossessionChance;

    if (bBarWon)
    {
        float Bonus = FMath::Clamp(NormalizedTimeLeft / 10.0f, 0.0f, 0.1f);
        FinalChance = FMath::Clamp(FinalChance + Bonus, 0.0f, 1.0f);
        bSuccess = FMath::FRand() < FinalChance;
    }
    else
    {
        TriggerEjectionFX(CachedAnimalSocketData.SocketGameplayTag);
        return;
    }

    int32 PulseCount = (FinalChance >= 0.66f) ? 2 : 1;
    if (bSuccess) PulseCount = 3;

    for (int32 Index = 0; Index < PulseCount; ++Index)
    {
        float Delay = Index * PulseDuration;
        FTimerHandle PulseHandle;
        GetWorld()->GetTimerManager().SetTimer(
            PulseHandle,
            [ASC = GetAbilitySystemComponentFromActorInfo(), Animal = TargetAnimalPtr]() {
                FGameplayCueParameters Params;
                Params.SourceObject = Animal.Get();
                Params.RawMagnitude = 1.f;
                ASC->ExecuteGameplayCue(FTBTags::Get().GameplayCue_Possession_Pulse, Params);
            },
            Delay,
            false);
    }

    float ResolveDelay = PulseCount * PulseDuration + CaptureDelay;
    FTimerHandle FinishHandle;
    GetWorld()->GetTimerManager().SetTimer(
        FinishHandle,
        [this, bSuccess]() {
            ACharacter* Parasite = Cast<ACharacter>(GetAvatarActorFromActorInfo());
            if (bSuccess && Parasite && PossessSuccessMontage)
            {
                Parasite->PlayAnimMontage(PossessSuccessMontage);
            }

            float AnimDelay = PossessSuccessMontage ? PossessSuccessMontage->GetPlayLength() : 0.5f;
            FTimerHandle PossessHandle;
            GetWorld()->GetTimerManager().SetTimer(PossessHandle, [this]() {
                if (TargetAnimalPtr.IsValid())
                {
                    AController* PC = GetActorInfo().PlayerController.Get();
                    const FGameplayAbilityActivationInfo& ActInfo = GetCurrentActivationInfo();
                    if (HasAuthority(&ActInfo))
                    {
                        GetActorInfo().AvatarActor->Destroy();
                        PC->Possess(TargetAnimalPtr.Get());
                    }
                }
                EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
            }, AnimDelay, false);
        },
        ResolveDelay,
        false);
}

/*  Clean‑up when minigame ends or ability cancels */
void UPossessHostAbility::EndBurrowCue()
{
    if (bHasBurrowEnded) return;
    bHasBurrowEnded = true;
    
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (ASC)
    {
        ASC->RemoveGameplayCue(FTBGameplayTags::Get().GameplayCue_Possession_BurrowingIn); // fires OnRemove
    }

    // CLEANUP
    if (GetActorInfo().IsNetAuthority())
    {
        if (AParasiteCharacter* ParasiteChar = Cast<AParasiteCharacter>(GetAvatarActorFromActorInfo()))
        {
            // Multicast function because collision isn't auto replicated
            if (TargetAnimalPtr.IsValid() && CachedAnimalSocketData.SocketGameplayTag.IsValid())
            {
                ParasiteChar->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                ParasiteChar->AddActorWorldOffset(
                FVector(0,0, ParasiteChar->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()+1.f));
                
                // Restore an upright orientation
                ParasiteChar->SetActorRotation(
                    FRotator(0.f, 0, 0.f),                              // pitch
                    ETeleportType::TeleportPhysics);                      // no interpolation
                
                ParasiteChar->SetBurrowCollision(true, TargetAnimalPtr.Get());
            }

            UCharacterMovementComponent* Move = ParasiteChar->GetCharacterMovement();
            Move->SetMovementMode(MOVE_Walking);

            ParasiteChar->ForceNetUpdate();
        }
    }
}

/* TriggerEjectionFX */
void UPossessHostAbility::TriggerEjectionFX(const FGameplayTag& SocketTag)
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

    const FGameplayTag PoopCue   = FTBTags::Get().GameplayCue_Possession_Eject_Poop;
    const FGameplayTag VomitCue  = FTBTags::Get().GameplayCue_Possession_Eject_Vomit;

    bool bButtSocket = SocketTag == FTBTags::Get().Sockets_Butt;

    FGameplayCueParameters Params;
    Params.SourceObject = TargetAnimalPtr.Get();
    Params.Location     = TargetAnimalPtr->GetCurrentSocketLocation(SocketTag);

    ASC->ExecuteGameplayCue(bButtSocket ? PoopCue : VomitCue, Params);

    ACharacter* Parasite = Cast<ACharacter>(GetAvatarActorFromActorInfo());

    if (Parasite)
    {
        FVector ImpulseDir = bButtSocket ?
       -TargetAnimalPtr->GetActorForwardVector() :
        TargetAnimalPtr->GetActorForwardVector();

        EndBurrowCue();
        Parasite->LaunchCharacter(ImpulseDir * 600.f, true, true);
    
    
        // Set view back to focusing on the parasite
        APlayerController* PC = GetActorInfo().PlayerController.Get();
        if (PC)
        {
            PC->SetViewTargetWithBlend(PC->GetPawn(), 0.25f, VTBlend_Cubic);
        }
    }
        
    
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
    
    // Leave this here if ability gets cancelled
    EndBurrowCue();
    TransferParasitePostProcessToAnimal(/*bApply=*/false);
    
    // Set input back to game
    ActorInfo->PlayerController->SetInputMode(FInputModeGameOnly());
    ActorInfo->PlayerController->SetShowMouseCursor(false);
    
    Super::EndAbilityCleanup(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
