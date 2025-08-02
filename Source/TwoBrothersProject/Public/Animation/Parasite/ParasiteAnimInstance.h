#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ParasiteAnimInstance.generated.h"

class AParasiteCharacter;

UCLASS(Blueprintable, Transient)
class TWOBROTHERSPROJECT_API UParasiteAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float Speed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float VerticalVelocity = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsInAir = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bJumpTriggered = false;
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	TWeakObjectPtr<AParasiteCharacter> Parasite;
};
