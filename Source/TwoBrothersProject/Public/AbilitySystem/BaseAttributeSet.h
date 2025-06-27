#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Data/CharacterCombatValues.h"
#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class UBaseAbilitySystemComponent;

template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

USTRUCT()
struct FTagAttributeBinding
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag PrimaryTag;

	UPROPERTY()
	FGameplayTag SecondaryTag; // Optional

	TStaticFuncPtr<FGameplayAttribute()> PrimaryAttributeFunc = nullptr;
	TStaticFuncPtr<FGameplayAttribute()> SecondaryAttributeFunc = nullptr;

	FTagAttributeBinding() = default;
	FTagAttributeBinding(
		const FGameplayTag& InPrimaryTag,
		TStaticFuncPtr<FGameplayAttribute()> InPrimaryFunc,
		const FGameplayTag& InSecondaryTag = FGameplayTag(),
		TStaticFuncPtr<FGameplayAttribute()> InSecondaryFunc = nullptr)
		: PrimaryTag(InPrimaryTag),
		  SecondaryTag(InSecondaryTag),
		  PrimaryAttributeFunc(InPrimaryFunc),
		  SecondaryAttributeFunc(InSecondaryFunc)
	{
	}
	
	bool HasSecondary() const { return SecondaryTag.IsValid(); }
};

UCLASS()
class TWOBROTHERSPROJECT_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UBaseAttributeSet();
	float CalculateCombatPower() const;
	
	TArray<FTagAttributeBinding> TagsToAttributes;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Energy)
	FGameplayAttributeData Energy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxEnergy)
	FGameplayAttributeData MaxEnergy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Oxygen)
	FGameplayAttributeData Oxygen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxOxygen)
	FGameplayAttributeData MaxOxygen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_AuraStrength)
	FGameplayAttributeData AuraStrength;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Defense)
	FGameplayAttributeData Defense;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_AuraDefense)
	FGameplayAttributeData AuraDefense;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Speed)
	FGameplayAttributeData Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Drowsiness)
	FGameplayAttributeData Drowsiness;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxDrowsiness)
	FGameplayAttributeData MaxDrowsiness;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Type)
	FGameplayAttributeData Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;

	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth);
	
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Energy);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxEnergy);
	
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Oxygen);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxOxygen);
	
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Strength);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, AuraStrength);
	
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Defense);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, AuraDefense);
	
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Speed);
	
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Drowsiness);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxDrowsiness);
	
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Type);
	
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, IncomingXP);
	
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, IncomingDamage);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Energy(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxEnergy(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Oxygen(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxOxygen(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_AuraStrength(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_AuraDefense(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Speed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Drowsiness(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxDrowsiness(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Type(const FGameplayAttributeData& OldValue);
	

private:
	UBaseAbilitySystemComponent* GetTBAbilitySystemComponent() const;
};
