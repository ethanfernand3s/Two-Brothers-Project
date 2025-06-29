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
		const FString FullTag = Data.SocketGameplayTag.ToString();   // "Socket.Head"

		// Grab the part after the final '.'
		FString Leaf;
		FullTag.Split(TEXT("."),       // delimiter
					  nullptr,         // throw away left part
					  &Leaf,           // keep right part
					  ESearchCase::IgnoreCase,
					  ESearchDir::FromEnd);

		// Now Leaf == "Head"
		const FName SocketName(*Leaf);
		if (!MeshComp->DoesSocketExist(SocketName)) continue;

		UWidgetComponent* WidgetComp = NewObject<UWidgetComponent>(GetOwner());
		WidgetComp->RegisterComponent();
		WidgetComp->SetActive(false);
		WidgetComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
		WidgetComp->SetWidgetClass(WidgetClass);
		WidgetComp->SetDrawAtDesiredSize(true);
		WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

		

		if (UPossessionChanceUserWidget* Widget = Cast<UPossessionChanceUserWidget>(WidgetComp->GetUserWidgetObject()))
		{
			Widget->SetPossessionChance(Data.PossessionChance);
		}

		SocketWidgetMap.Add(SocketName, WidgetComp);
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

void UAnimalExtensionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAnimalExtensionComponent, bAttributesInitialised);
}

