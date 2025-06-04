#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class UBaseAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttributeChangedEvent, const UAttributeSet*, AttributeSet, float, OldValue, float, NewValue);

UCLASS()
class TWOBROTHERSPROJECT_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Oxygen)
	FGameplayAttributeData Oxygen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxOxygen)
	FGameplayAttributeData MaxOxygen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Hunger)
	FGameplayAttributeData Hunger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHunger)
	FGameplayAttributeData MaxHunger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Thirst)
	FGameplayAttributeData Thirst;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxThirst)
	FGameplayAttributeData MaxThirst;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Weight)
	FGameplayAttributeData Weight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxWeight)
	FGameplayAttributeData MaxWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMovementSpeed)
	FGameplayAttributeData MaxMovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Drowsiness)
	FGameplayAttributeData Drowsiness;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Type)
	FGameplayAttributeData Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Level)
	FGameplayAttributeData Level;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Damage;

	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Oxygen);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxOxygen);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Hunger);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHunger);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Thirst);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxThirst);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Weight);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxWeight);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Type);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Level);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MovementSpeed);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxMovementSpeed);
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Drowsiness);

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedEvent OnHealthChanged;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Oxygen(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxOxygen(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Hunger(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxHunger(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Thirst(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxThirst(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Weight(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxWeight(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MovementSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Drowsiness(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Type(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Level(const FGameplayAttributeData& OldValue);
	

private:
	UBaseAbilitySystemComponent* GetTBAbilitySystemComponent() const;
};
