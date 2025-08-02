#include "Animation/Parasite/ParasiteAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/ParasiteCharacter.h"

void UParasiteAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* PawnOwner = TryGetPawnOwner();
	if (PawnOwner)
	{
		Parasite = Cast<AParasiteCharacter>(PawnOwner);
	}
}

void UParasiteAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Parasite.IsValid())
	{
		APawn* PawnOwner = TryGetPawnOwner();
		if (PawnOwner)
		{
			Parasite = Cast<AParasiteCharacter>(PawnOwner);
		}
	}

	if (!Parasite.IsValid())
	{
		Speed = 0.f;
		VerticalVelocity = 0.f;
		bIsInAir = false;
		return;
	}

	const FVector Velocity = Parasite->GetVelocity();
	Speed = FVector(Velocity.X, Velocity.Y, 0.f).Size();
	VerticalVelocity = Velocity.Z;

	// Optional: use character movement for reliable state
	if (const UCharacterMovementComponent* MoveComp = Parasite->GetCharacterMovement())
	{
		bIsInAir = MoveComp->IsFalling();
	}
	else
	{
		bIsInAir = false;
	}
}
