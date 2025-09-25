// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Base/BaseAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "TBGameplayTags.h"
#include "Characters/Components/CharacterContextComponent.h"
#include "Characters/Data/LevelInfo.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Player/ParasitePlayerState.h"
#include "Player/Interfaces/PlayerInterface.h"

UBaseAttributeSet::UBaseAttributeSet()
{
	const FTBGameplayTags& GameplayTags = FTBGameplayTags::Get();

	// TagsToSurvivalAttributes
	
	TagsToSurvivalAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_Health,
		&UBaseAttributeSet::GetHealthAttribute,
		GameplayTags.Attributes_MaxHealth,
		&UBaseAttributeSet::GetMaxHealthAttribute
	));

	TagsToSurvivalAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_Energy,
		&UBaseAttributeSet::GetEnergyAttribute,
		GameplayTags.Attributes_MaxEnergy,
		&UBaseAttributeSet::GetMaxEnergyAttribute
	));

	TagsToSurvivalAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_Oxygen,
		&UBaseAttributeSet::GetOxygenAttribute,
		GameplayTags.Attributes_MaxOxygen,
		&UBaseAttributeSet::GetMaxOxygenAttribute
	));
	

	TagsToSurvivalAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_Drowsiness,
		&UBaseAttributeSet::GetDrowsinessAttribute,
		GameplayTags.Attributes_MaxDrowsiness,
		&UBaseAttributeSet::GetMaxDrowsinessAttribute
	));

	// TagsToCombatAttributes

	TagsToCombatAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_Health,
		&UBaseAttributeSet::GetHealthAttribute,
		GameplayTags.Attributes_MaxHealth,
		&UBaseAttributeSet::GetMaxHealthAttribute
	));

	TagsToCombatAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_Strength,
		&UBaseAttributeSet::GetStrengthAttribute
	));

	TagsToCombatAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_AuraStrength,
		&UBaseAttributeSet::GetAuraStrengthAttribute
	));

	TagsToCombatAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_Defense,
		&UBaseAttributeSet::GetDefenseAttribute
	));

	TagsToCombatAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_AuraDefense,
		&UBaseAttributeSet::GetAuraDefenseAttribute
	));

	TagsToCombatAttributes.Add(FTagAttributeBinding(
		GameplayTags.Attributes_Speed,
		&UBaseAttributeSet::GetSpeedAttribute
	));
}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBaseAttributeSet, Health);
	DOREPLIFETIME(UBaseAttributeSet, MaxHealth);
	DOREPLIFETIME(UBaseAttributeSet, Energy);
	DOREPLIFETIME(UBaseAttributeSet, MaxEnergy);
	DOREPLIFETIME(UBaseAttributeSet, Oxygen);
	DOREPLIFETIME(UBaseAttributeSet, MaxOxygen);
	DOREPLIFETIME(UBaseAttributeSet, Strength);
	DOREPLIFETIME(UBaseAttributeSet, AuraStrength);
	DOREPLIFETIME(UBaseAttributeSet, Defense);
	DOREPLIFETIME(UBaseAttributeSet, AuraDefense);
	DOREPLIFETIME(UBaseAttributeSet, Speed);
	DOREPLIFETIME(UBaseAttributeSet, Drowsiness);
	DOREPLIFETIME(UBaseAttributeSet, MaxDrowsiness);
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}
	else if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
	{
		SetEnergy(FMath::Clamp(GetEnergy(), 0, GetMaxEnergy()));
	}
	else if (Data.EvaluatedData.Attribute == GetOxygenAttribute())
	{
		SetOxygen(FMath::Clamp(GetOxygen(), 0, GetMaxOxygen()));
	}
	else if (Data.EvaluatedData.Attribute == GetDrowsinessAttribute())
	{
		SetDrowsiness(FMath::Clamp(GetDrowsiness(), 0, GetMaxDrowsiness()));
	}
}

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		SetHealth(FMath::Clamp(NewValue, 0.f, GetMaxHealth()));
	}
	else if (Attribute == GetMaxEnergyAttribute())
	{
		SetEnergy(FMath::Clamp(NewValue, 0.f, GetMaxEnergy()));
	}
	else if (Attribute == GetMaxOxygenAttribute())
	{
		SetOxygen(FMath::Clamp(NewValue, 0.f, GetMaxOxygen()));
	}
	else if (Attribute == GetMaxDrowsinessAttribute())
	{
		SetDrowsiness(FMath::Clamp(NewValue, 0.f, GetMaxDrowsiness()));
	}
}

void UBaseAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	UE_LOG(LogTemp, Warning, TEXT("PostChange: Attribute '%s' changed %.2f -> %.2f"), *Attribute.AttributeName, OldValue, NewValue);
	
	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
		bTopOffHealth = false;
	}
	else if (Attribute == GetMaxEnergyAttribute() && bTopOffEnergy)
	{
		SetEnergy(FMath::Clamp(GetEnergy(), 0, GetMaxEnergy()));
		bTopOffEnergy = false;
	}
	else if (Attribute == GetMaxOxygenAttribute() && bTopOffOxygen)
	{
		SetOxygen(FMath::Clamp(GetOxygen(), 0, GetMaxOxygen()));
		bTopOffOxygen = false;
	}
	else if (Attribute == GetMaxDrowsinessAttribute() && bTopOffDrowsiness)
	{
		SetDrowsiness(FMath::Clamp(GetDrowsiness(), 0, GetMaxDrowsiness()));
		bTopOffDrowsiness = false;
	}
}

void UBaseAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = causer of the effect, Target = target of the effect (owner of this AS)

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UBaseAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			/*
			 * TODO: Implement CombatInterface + Dieing */
			 /* 
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
			if (CombatInterface)
			{
				FVector Impulse = UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle);
				CombatInterface->Die(UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));
			}
			*/
			
			SendXPEvent(Props);
			
		}
		else
		{
			/*
			 * TODO: Implement Knockback */
			 /* 
			if (Props.TargetCharacter->Implements<UCombatInterface>())
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FTBGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			
			const FVector& KnockbackForce = UAbilitySystemLibrary::GetKnockbackForce(Props.EffectContextHandle);
			if (!KnockbackForce.IsNearlyZero(1.f))
			{
				Props.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
			}
			*/
		}

		/*
		 * TODO: Implement Blocking and Crit Hits */
		 /* 
		const bool bBlock = UAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bCriticalHit = UAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCriticalHit);
		*/

		/*
		 * TODO: Add debuffs */
		 /* 
		if (UAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
		{
			Debuff(Props);
		}
		*/
	}
}

void UBaseAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);

	// Source Character is the owner, since GA_ListenForEvents applies GE_EventBasedEffect, adding to IncomingXP
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(Props.SourceCharacter))
	{
		const int32 CurrentLevel = PlayerInterface->GetCharacterContextComponent()->GetLevel();
		const int32 CurrentXP = PlayerInterface->GetCharacterContextComponent()->GetXP();

		const int32 NewLevel = LevelInfoLibrary::GetLevelFromXP(CurrentXP + LocalIncomingXP, PlayerInterface->GetCharacterContextComponent()->GetGrowthRate());
		const int32 NumLevelUps = NewLevel - CurrentLevel;
		if (NumLevelUps > 0)
		{
			PlayerInterface->GetCharacterContextComponent()->AddToLevel(NumLevelUps);

			int32 AttributePointsReward = 0;

			for (int32 i = 0; i < NumLevelUps; ++i)
			{
				AttributePointsReward += 1; // Could make the attribute point reward dynamic and based on the characters level + 1
			}
			
			PlayerInterface->GetCharacterContextComponent()->AddToAttributePoints(AttributePointsReward);
			
			bTopOffHealth = true;
			bTopOffEnergy = true;
				
			// PlayerInterface->GetCharacterContextComponent()->LevelUp(); Could Implement if needed
		}
			
		PlayerInterface->GetCharacterContextComponent()->AddToXP(LocalIncomingXP);
	}
}

void UBaseAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(Props.SourceCharacter))
	{
		const int32 XPReward = LevelInfoLibrary::GetXPReward(PlayerInterface->GetCharacterContextComponent()->GetBaseXP(), PlayerInterface->GetCharacterContextComponent()->GetLevel(),
			PlayerInterface->GetIsInhabited(), PlayerInterface->GetXPMultiplierAmount());

		const FTBGameplayTags& GameplayTags = FTBGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXP, Payload);
	}
}

void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldValue);
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldValue);
}

void UBaseAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Energy, OldValue);
}

void UBaseAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxEnergy, OldValue);
}

void UBaseAttributeSet::OnRep_Oxygen(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Oxygen, OldValue);
}

void UBaseAttributeSet::OnRep_MaxOxygen(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxOxygen, OldValue);
}

void UBaseAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Strength, OldValue);
}

void UBaseAttributeSet::OnRep_AuraStrength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, AuraStrength, OldValue);
}

void UBaseAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Defense, OldValue);
}

void UBaseAttributeSet::OnRep_AuraDefense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, AuraDefense, OldValue);
}

void UBaseAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Speed, OldValue);
}

void UBaseAttributeSet::OnRep_Drowsiness(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Drowsiness, OldValue);
}

void UBaseAttributeSet::OnRep_MaxDrowsiness(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxDrowsiness, OldValue);
}

float UBaseAttributeSet:: CalculateCombatPower() const
{
	return (GetMaxHealth() + GetStrength() + GetDefense() + GetAuraStrength()+ GetAuraDefense() + GetSpeed());
}
