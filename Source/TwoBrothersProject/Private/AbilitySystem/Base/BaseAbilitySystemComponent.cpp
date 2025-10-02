// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Base/BaseAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TBGameplayTags.h"
#include "AbilitySystem/Base/Abilities/BaseGameplayAbility.h"
#include "AbilitySystem/Data/CharacterCombatValues.h"
#include "Characters/Components/CharacterContextComponent.h"
#include "Player/Interfaces/PlayerInterface.h"

void UBaseAbilitySystemComponent::HandleAbilityStatusChanged(const UTBInventoryItem* Item, FGameplayTag SlotInputTag)
{
	Server_HandleAbilityStatusChanged(Item, SlotInputTag);
}

void UBaseAbilitySystemComponent::Server_HandleAbilityStatusChanged_Implementation(const UTBInventoryItem* Item,
	FGameplayTag SlotInputTag)
{
	if (!IsValid(Item)) return;
	
	const auto& Tags = FTBGameplayTags::Get();
	
	const FAbilityFragment* AbilityFragment = GetFragment<FAbilityFragment>(Item, Tags.Fragments_Ability);
	if (!AbilityFragment) return;

	const FGameplayTag& AbilityType = Item->GetPreferredSlotContainerTag();
	bool bIsPassiveAbility = (AbilityType == Tags.ItemCategories_Abilities_Passive);
	
	const FGameplayTag& NewItemStatus = Item->GetItemStatus();
	const FGameplayTag& AbilityTag = AbilityFragment->GetAbilityTag();
	FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag);
	
	if (AbilitySpec) // The ability has already been given to the ASC
	{
		if (NewItemStatus == Tags.Status_Unlocked)
		{
			UnEquipAbility(AbilitySpec, bIsPassiveAbility);
		}
		else if (NewItemStatus == Tags.Status_Equipped)
		{
			if (SlotInputTag.IsValid()) EquipAbility(AbilitySpec, SlotInputTag, bIsPassiveAbility);
		}
		else // New status is locked or none
		{
			ClearAbility(AbilitySpec->Handle);
		}
	}
	else if (NewItemStatus == Tags.Status_Unlocked || NewItemStatus == Tags.Status_Equipped)
	{
		// The ability hasn't been given to the ASC
		const FLevelFragment* LevelFragment = GetFragment<FLevelFragment>(Item, Tags.Fragments_Level);
		if (!LevelFragment) return;
		int32 AbilityLevel = LevelFragment->GetCurrentLevel();
	
		// Optional 
		const FCreatureTypeFragment* CreatureTypeFragment = GetFragment<FCreatureTypeFragment>(Item, Tags.Fragments_CreatureType);
		const FGameplayTag& CreatureTypeTag = (CreatureTypeFragment) ? CreatureTypeFragment->GetAbilityCreatureTypeTag() : FGameplayTag::EmptyTag;
		/* Note: Could also add the rarity fragment if we want the rarity to influence the effectiveness of the ability. */
		
		const TSubclassOf<UBaseGameplayAbility>& AbilityClass = AbilityFragment->GetAbilityClass();
		
		FGameplayAbilitySpecHandle NewSpecHandle = UnlockAbility(AbilityClass, AbilityLevel, NewItemStatus, CreatureTypeTag);
		if (NewItemStatus == Tags.Status_Equipped)
		{
			if (FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(NewSpecHandle))
			{
				if (SlotInputTag.IsValid())  EquipAbility(Spec, SlotInputTag, bIsPassiveAbility);
			}
		}
	}
}

FGameplayAbilitySpecHandle UBaseAbilitySystemComponent::UnlockAbility(
	const TSubclassOf<UBaseGameplayAbility>& AbilityClass,
	int32 AbilityLevel,
	const FGameplayTag& NewStatus,
	const FGameplayTag& CreatureTypeTag)
{
	if (!AbilityClass) return FGameplayAbilitySpecHandle();

	FGameplayAbilitySpec NewSpec(AbilityClass, AbilityLevel);

	// Add dynamic tags for status and creature type
	if (NewStatus.IsValid())
	{
		NewSpec.GetDynamicSpecSourceTags().AddTag(NewStatus);
	}
	if (CreatureTypeTag.IsValid())
	{
		NewSpec.GetDynamicSpecSourceTags().AddTag(CreatureTypeTag);
	}

	// Grant to ASC
	FGameplayAbilitySpecHandle Handle = GiveAbility(NewSpec);
	if (Handle.IsValid())
	{
		MarkAbilitySpecDirty(NewSpec);
	}

	return Handle;
	// TODO: Come back to once local predicted approach is in place
	// ClientUpdateAbilityStatus(GetAbilityTagFromSpec(NewSpec),FTBGameplayTags::Get().Status_Unlocked, AbilityLevel);
}

void UBaseAbilitySystemComponent::EquipAbility(
	FGameplayAbilitySpec* AbilitySpec,
	const FGameplayTag& SlotInputTag,
	bool bIsPassiveAbility)
{
	if (!AbilitySpec || !SlotInputTag.IsValid()) return;
	const auto& Tags = FTBGameplayTags::Get();

	// Only valid if currently unlocked
	const FGameplayTag& OldStatus = GetStatusFromSpec(*AbilitySpec);
	if (OldStatus != Tags.Status_Unlocked && OldStatus != Tags.Status_Equipped)
	{
		return;
	}

	const FGameplayTag& AbilityTag = GetAbilityTagFromSpec(*AbilitySpec);
	if (!AbilityTag.IsValid()) return;
	
	// If slot already taken, unequip that ability
	if (!SlotIsEmpty(SlotInputTag))
	{
		if (FGameplayAbilitySpec* SpecWithSlot = GetSpecWithSlot(SlotInputTag))
		{
			// Same ability instance return early
			if (SpecWithSlot == AbilitySpec)
			{
				// Client notify // TODO: Come back to once local predicted approach is in place
				// ClientEquipAbility(AbilityTag, Tags.Abilities_Status_Equipped, Slot, PrevSlot);
				return;
			}
			
			UnEquipAbility(SpecWithSlot, bIsPassiveAbility);
		}
	}

	if (!AbilityHasAnySlot(*AbilitySpec)) // Ability doesn't yet have a slot (it's not active)
	{
		if (bIsPassiveAbility)
		{
			TryActivateAbility(AbilitySpec->Handle);
			MulticastActivatePassiveEffect(AbilityTag, true);
		}
		AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GetStatusFromSpec(*AbilitySpec));
		AbilitySpec->GetDynamicSpecSourceTags().AddTag(Tags.Status_Equipped);
	}
	AssignSlotToAbility(*AbilitySpec, SlotInputTag);
	MarkAbilitySpecDirty(*AbilitySpec);

	// Client notify // TODO: Come back to once local predicted approach is in place
	// ClientEquipAbility(AbilityTag, Tags.Status_Equipped, SlotInputTag, PreviousSlotInputTag); 
}


FGameplayAbilitySpec* UBaseAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->GetAssetTags())
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

FGameplayTag UBaseAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

bool UBaseAbilitySystemComponent::SlotIsEmpty(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(AbilitySpec, Slot))
		{
			return false;
		}
	}
	return true;
}

bool UBaseAbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	return Spec.GetDynamicSpecSourceTags().HasTagExact(Slot);
}

bool UBaseAbilitySystemComponent::AbilityHasAnySlot(const FGameplayAbilitySpec& Spec)
{
	return Spec.GetDynamicSpecSourceTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Inputs.Abilities")));
}

FGameplayAbilitySpec* UBaseAbilitySystemComponent::GetSpecWithSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(Slot))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

void UBaseAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	ClearSlot(&Spec);
	Spec.GetDynamicSpecSourceTags().AddTag(Slot);
}

void UBaseAbilitySystemComponent::Server_DirectUnEquipAbility_Implementation(const UTBInventoryItem* Item)
{
	if (!IsValid(Item)) return;
	
	const auto& Tags = FTBGameplayTags::Get();
	
	const FAbilityFragment* AbilityFragment = GetFragment<FAbilityFragment>(Item, Tags.Fragments_Ability);
	if (!AbilityFragment) return;
	
	const FGameplayTag& AbilityType = Item->GetPreferredSlotContainerTag();
	bool bIsPassiveAbility = (AbilityType == Tags.ItemCategories_Abilities_Passive);
	
	const FGameplayTag& AbilityTag = AbilityFragment->GetAbilityTag();
	FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag);

	if (AbilitySpec != nullptr)
	{
		UnEquipAbility(AbilitySpec, bIsPassiveAbility);
	}
}

void UBaseAbilitySystemComponent::UnEquipAbility(FGameplayAbilitySpec* AbilitySpec, bool bIsPassiveAbility)
{
	const FGameplayTag& OldItemStatus = GetStatusFromSpec(*AbilitySpec);
	if (OldItemStatus != FTBGameplayTags::Get().Status_Equipped) return;
	
	if (bIsPassiveAbility)
	{
		MulticastActivatePassiveEffect(GetAbilityTagFromSpec(*AbilitySpec), false);
		DeactivatePassiveEffect.Broadcast(GetAbilityTagFromSpec(*AbilitySpec));
	}
	ClearSlot(AbilitySpec);
	AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(OldItemStatus);
	AbilitySpec->GetDynamicSpecSourceTags().AddTag(FTBGameplayTags::Get().Status_Unlocked);

	MarkAbilitySpecDirty(*AbilitySpec);
}

void UBaseAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*Spec);
	Spec->GetDynamicSpecSourceTags().RemoveTag(Slot);
}

void UBaseAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag,
	bool bActivate)
{
	if (bActivate) ActivatePassiveEffect.Broadcast(AbilityTag);
	else DeactivatePassiveEffect.Broadcast(AbilityTag);
}

void UBaseAbilitySystemComponent::AddCharacterAbility(const TSubclassOf<UGameplayAbility>& GameplayAbilityClass,
                                                      int32 AbilityLevel, const FGameplayTag& InputTag)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GameplayAbilityClass, AbilityLevel);
	AbilitySpec.GetDynamicSpecSourceTags().AddTag(InputTag);
	GiveAbility(AbilitySpec);
	
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UBaseAbilitySystemComponent::AddCharacterPassiveAbility(const TSubclassOf<UGameplayAbility>& GameplayAbilityClass,
	int32 AbilityLevel)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GameplayAbilityClass, AbilityLevel);
	GiveAbilityAndActivateOnce(AbilitySpec);
}

void UBaseAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UBaseAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->GetAssetTags())
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}


FGameplayTag UBaseAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Inputs"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

// Commenting for now since the item will keep track of this
// FGameplayTag UBaseAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
// {
// 	for (FGameplayTag StatusTag : AbilitySpec.GetDynamicSpecSourceTags())
// 	{
// 		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
// 		{
// 			return StatusTag;
// 		}
// 	}
// 	return FGameplayTag();
// }

// TODO: Change these input functions to come from an item, therefore we can't check the AbilitySpec's
// Tag since it will be on the items Input Fragment
void UBaseAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				// TODO: Is Getting Primary Instance Of The Ability Spec Not All Instances, If This Becomes A Problem Fix It
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.GetPrimaryInstance()->GetCurrentActivationInfo().GetActivationPredictionKey());
			}
		}
	}
}

void UBaseAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UBaseAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			// TODO: Is Getting Primary Instance Of The Ability Spec Not All Instances, If This Becomes A Problem Fix It
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.GetPrimaryInstance()->GetCurrentActivationInfo().GetActivationPredictionKey());
		}
	}
}

void UBaseAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	Server_UpgradeAttribute(AttributeTag);
}

void UBaseAbilitySystemComponent::Server_UpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	AActor* CurrentAvatarActor = GetAvatarActor();
	if (CurrentAvatarActor && CurrentAvatarActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(CurrentAvatarActor);

		if (PlayerInterface && PlayerInterface->GetCharacterContextComponent()->GetAttributePoints() > 0)
		{
			// Send Gameplay Event
			FGameplayEventData Payload;
			Payload.EventTag = AttributeTag;
			Payload.EventMagnitude = 1.f;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CurrentAvatarActor, AttributeTag, Payload);

			// Subtract Attribute Point
			PlayerInterface->GetCharacterContextComponent()->AddToAttributePoints(-1);
		}
	}
}

void UBaseAbilitySystemComponent::SetBaseStats(float CurrentCombatPower, int32 Level)
{
	constexpr float Multiplier = 1.045f;              // Growth rate per level
	CurrentCombatPower *= FMath::Pow(Multiplier, Level - 1);
	
	// Generate wild procedural stats from CP
	TArray<float> RawWeights;
	for (int i = 0; i < 6; ++i)
	{
		RawWeights.Add(FMath::FRandRange(0.1f, 2.0f)); // More room for extreme bias
	}

	// Normalize weights
	float WeightSum = 0.f;
	for (float W : RawWeights) WeightSum += W;
	for (float& W : RawWeights) W /= WeightSum;

	// Convert to final stat values
	TArray<float> StatValues;
	int32 Allocated = 0;
	for (float W : RawWeights)
	{
		float Value = CurrentCombatPower * W;
		StatValues.Add(Value);
		Allocated += FMath::RoundToInt(Value);
	}

	// Distribute Remainder
	int32 Remainder = FMath::RoundToInt(CurrentCombatPower) - Allocated;
	while (Remainder > 0)
	{
		int Index = FMath::RandRange(0, 5);
		StatValues[Index] += 1.f;
		--Remainder;
	}

	// Assign combat stats
	FCharacterCombatValues CombatValues;
	CombatValues.Health = StatValues[0];
	CombatValues.Strength = StatValues[1];
	CombatValues.Defense = StatValues[2];
	CombatValues.AuraStrength = StatValues[3];
	CombatValues.AuraDefense = StatValues[4];
	CombatValues.Speed = StatValues[5];

	SetBaseStats(CombatValues);
}

void UBaseAbilitySystemComponent::SetBaseStats(const FCharacterCombatValues& CustomCombatValues)
{
	FGameplayEffectSpecHandle SpecH =
		MakeOutgoingSpec(BaseStatsGameplayEffect, 1.f, MakeEffectContext());

	FGameplayEffectSpec* Spec = SpecH.Data.Get();
	if (!Spec) return;

	const static FTBGameplayTags GameplayTags = FTBGameplayTags::Get();

	// @Note: Current Attributes are clamped to max in attribute set
	
	// Survival Stats
	// TODO: Change later to have these be different with each animal
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_MaxAura, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_MaxOxygen, 100.f);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_MaxDrowsiness, 100.f);

	// Combat Stats
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_MaxHealth, CustomCombatValues.Health);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Strength, CustomCombatValues.Strength);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_AuraStrength, CustomCombatValues.AuraStrength);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Defense, CustomCombatValues.Defense);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_AuraDefense, CustomCombatValues.AuraDefense);
	Spec->SetSetByCallerMagnitude(GameplayTags.Attributes_Speed, CustomCombatValues.Speed);

	ApplyGameplayEffectSpecToSelf(*Spec);
}

void UBaseAbilitySystemComponent::AddIvsToAttributes(const FCharacterCombatValues& CharIvSet)
{
	FGameplayEffectSpecHandle SpecH =
		MakeOutgoingSpec(ApplyIvsGameplayEffect, 1.f, MakeEffectContext());

	FGameplayEffectSpec* Spec = SpecH.Data.Get();
	if (!Spec) return;

	const static FTBGameplayTags GameplayTags = FTBGameplayTags::Get();

	// @Note: Current Attributes are clamped to max in attribute set
	
	// Combat Stats
	Spec->SetSetByCallerMagnitude(
		GameplayTags.Attributes_MaxHealth, CharIvSet.Health);

	Spec->SetSetByCallerMagnitude(
		GameplayTags.Attributes_Strength, CharIvSet.Strength);
	Spec->SetSetByCallerMagnitude(
			GameplayTags.Attributes_AuraStrength, CharIvSet.AuraStrength);
	
	Spec->SetSetByCallerMagnitude(
		GameplayTags.Attributes_Defense, CharIvSet.Defense);
	Spec->SetSetByCallerMagnitude(
		GameplayTags.Attributes_AuraDefense, CharIvSet.AuraDefense);
	
	Spec->SetSetByCallerMagnitude(
		GameplayTags.Attributes_Speed, CharIvSet.Speed);
	
	ApplyGameplayEffectSpecToSelf(*Spec);
}

void UBaseAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}