#pragma once
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "PossessResultTargetData.generated.h"

USTRUCT()
struct FGameplayAbilityTargetData_PossessResult : public FGameplayAbilityTargetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	uint8 bWon : 1;            // 1 = win, 0 = loss

	UPROPERTY()
	float NormalizedTimeLeft{0.f};        // 0-1 time left

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
		uint8 Flag = bWon;

		/* write or read the single bit */
		Ar.SerializeBits(&Flag, 1);

		/* copy back after loading */
		if (Ar.IsLoading())
		{
			bWon = Flag & 1;
		}

		/* 32-bit float always serialised */
		Ar << NormalizedTimeLeft;

		bOutSuccess = true;
		return true;
	}
};

/* Tell Unreal that this struct has a native NetSerialize */
template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_PossessResult>
	: public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_PossessResult>
{
	enum
	{
		WithNetSerializer = true,   // enables custom NetSerialize
		WithCopy          = true
	};
};