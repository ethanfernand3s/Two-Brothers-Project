#pragma once
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "BurrowDepthUpdateTargetData.generated.h"

USTRUCT()
struct FGameplayAbilityTargetData_BurrowDepthUpdate : public FGameplayAbilityTargetData
{
	GENERATED_USTRUCT_BODY() 

	UPROPERTY()
	float NewDepth;

	/* Required override so the handle can identify this struct type */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	/* This struct carries no hit result */
	virtual bool HasHitResult() const override { return false; }

	/* -------- NetSerialize -------- */
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		/* 32-bit float always serialised */
		Ar << NewDepth;

		bOutSuccess = true;
		return true;
	}
};

/* Tell Unreal that this struct has a native NetSerialize */
template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_BurrowDepthUpdate>
	: public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_BurrowDepthUpdate>
{
	enum
	{
		WithNetSerializer = true,   // enables custom NetSerialize
		WithCopy          = true
	};
};