// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AnimalExtensionComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitiesModule.h"
#include "GameplayEffect.h"
#include "AbilitySystem/Animal/AnimalAbilitySet.h"
#include "Characters/BaseAnimalCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Player/ParasitePlayerState.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/Possession/PossessionChanceUserWidget.h"


// Sets default values for this component's properties
UAnimalExtensionComponent::UAnimalExtensionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAttributesInitialised = false;
}

void UAnimalExtensionComponent::InitializePossessionWidgets(const TArray<FPossessionSocketData>& SocketData, TSubclassOf<UPossessionChanceUserWidget> WidgetClass, USkeletalMeshComponent* MeshComp)
{
	if (!WidgetClass || !MeshComp) return;

	for (const FPossessionSocketData& Data : SocketData)
	{
		if (!MeshComp->DoesSocketExist(Data.SocketName)) continue;

		UWidgetComponent* WidgetComp = NewObject<UWidgetComponent>(GetOwner());
		WidgetComp->RegisterComponent();
		WidgetComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, Data.SocketName);
		WidgetComp->SetWidgetClass(WidgetClass);
		WidgetComp->SetDrawAtDesiredSize(true);
		WidgetComp->SetWidgetSpace(EWidgetSpace::World);
		WidgetComp->SetTwoSided(true);

		if (UPossessionChanceUserWidget* Widget = Cast<UPossessionChanceUserWidget>(WidgetComp->GetUserWidgetObject()))
		{
			Widget->SetPossessionChance(Data.PossessionChance);
		}

		SocketWidgetMap.Add(Data.SocketName, WidgetComp);
	}
}

void UAnimalExtensionComponent::UpdatePossessionChance(FName SocketName, float NewChance)
{
	if (UWidgetComponent* WidgetComp = SocketWidgetMap.FindRef(SocketName))
	{
		if (UPossessionChanceUserWidget* Widget = Cast<UPossessionChanceUserWidget>(WidgetComp->GetUserWidgetObject()))
		{
			Widget->SetPossessionChance(NewChance);
		}
	}
}
void UAnimalExtensionComponent::HandlePlayerPossess(AParasitePlayerState* OwnerPS)
{
	CachedASC = GetOwner<IAbilitySystemInterface>()->GetAbilitySystemComponent();
	if (!CachedASC) return;

	// Ensure parasite and animal initial attributes
	OwnerPS->EnsureInitialAttributeDefaults();
	EnsureInitialAttributeDefaults();
	
	// Apply parasite buff GE to host
	for (const UGameplayEffect* ParasiteBuff : OwnerPS->GetBuffForHost()) // implement helper
	{
		FGameplayEffectContextHandle Ctx = CachedASC->MakeEffectContext();
		Ctx.AddSourceObject(OwnerPS);
		// Applies GE on Host
		CachedASC->ApplyGameplayEffectToSelf(ParasiteBuff, 1, Ctx);
	}

	if (AnimalAbilitySet && GrantedHandles.Num() == 0)
	{
		AnimalAbilitySet->GiveToAbilitySystem(CachedASC, &GrantedHandles);
	}
}

void UAnimalExtensionComponent::HandlePlayerUnPossess()
{
	if (!CachedASC) return;

	// Remove parasite buff (by tag)
	/* TODO: Create this tag / effect
	// CachedASC->RemoveActiveEffectsWithTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Effect.Buff.Parasite"))));
	*/
}

void UAnimalExtensionComponent::EnsureInitialAttributeDefaults()
{
	if (!CachedASC)
	{
		CachedASC = GetOwner<IAbilitySystemInterface>()->GetAbilitySystemComponent();
	}
	
	if (bAttributesInitialised || !CachedASC) return;

	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults
	(CachedASC, "Animal", /*Level=*/1, /*IsInitialLoad=*/true);
	bAttributesInitialised = true;
}

void UAnimalExtensionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAnimalExtensionComponent, bAttributesInitialised);
}

