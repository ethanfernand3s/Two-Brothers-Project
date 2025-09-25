#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "TBBodyPartRules.generated.h"

USTRUCT(BlueprintType)
struct FTBBodyPartSockets
{
	GENERATED_BODY()

	// e.g., Sockets.Ear -> ["Ear_1","Ear_2"]
	UPROPERTY(EditAnywhere) FGameplayTag BodyPartTag;

	UPROPERTY(EditAnywhere) TArray<FName> SocketNames;
};

UCLASS()
class TWOBROTHERSPROJECT_API UTBBodyPartRules : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere) TArray<FTBBodyPartSockets> Rules;

	bool HasRequiredSockets(const class USkeletalMeshComponent* Skel, const FGameplayTag& BodyPartTag) const;
};
