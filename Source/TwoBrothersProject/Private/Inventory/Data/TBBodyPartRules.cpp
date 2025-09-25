#include "Inventory/Data/TBBodyPartRules.h"
#include "Components/SkeletalMeshComponent.h"

bool UTBBodyPartRules::HasRequiredSockets(const USkeletalMeshComponent* Skel, const FGameplayTag& BodyPartTag) const
{
	if (!Skel || !BodyPartTag.IsValid()) return true;
	for (const auto& R : Rules)
	{
		if (R.BodyPartTag == BodyPartTag)
		{
			for (const FName& S : R.SocketNames)
			{
				if (!Skel->DoesSocketExist(S)) return false;
			}
			return true;
		}
	}
	return true;
}
