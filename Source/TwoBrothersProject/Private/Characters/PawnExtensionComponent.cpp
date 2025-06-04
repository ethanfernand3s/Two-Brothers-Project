// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnExtensionComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitiesModule.h"
#include "GameplayEffect.h"
#include "AbilitySystem/Animal/AnimalAbilitySet.h"
#include "Net/UnrealNetwork.h"
#include "Player/ParasitePlayerState.h"


// Sets default values for this component's properties
UPawnExtensionComponent::UPawnExtensionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAttributesInitialised = false;
}

void UPawnExtensionComponent::HandlePlayerPossess(AParasitePlayerState* OwnerPS)
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

void UPawnExtensionComponent::HandlePlayerUnPossess()
{
	if (!CachedASC) return;

	// Remove parasite buff (by tag)
	/* TODO: Create this tag / effect
	// CachedASC->RemoveActiveEffectsWithTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Effect.Buff.Parasite"))));
	*/
}

void UPawnExtensionComponent::EnsureInitialAttributeDefaults()
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

void UPawnExtensionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPawnExtensionComponent, bAttributesInitialised);
}

