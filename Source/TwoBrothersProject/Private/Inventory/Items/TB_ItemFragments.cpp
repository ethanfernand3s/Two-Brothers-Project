#include "Inventory/Items/TB_ItemFragments.h"
#include "UI/Widget/Composite/CompositeBase.h"
#include "UI/Widget/Composite/Leaf/Leaf_Image.h"
#include "UI/Widget/Composite/Leaf/Leaf_Text.h"

void FInventoryItemFragment::Assimilate(UCompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite)) return;
	Composite->Expand();
}

bool FInventoryItemFragment::MatchesWidgetTag(const UCompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}

void FTextFragment::Assimilate(UCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);

	ULeaf_Text* LeafText = Cast<ULeaf_Text>(Composite);
	if (!IsValid(LeafText)) return;

	LeafText->SetText(FragmentText);
	LeafText->SetTextColor(FragmentTextColor);
}

bool FRequirementsFragment::MeetsItemRequirements(int32 CreatureLevel, const FGameplayTagContainer& CreatureBodyPartSockets,
	const FGameplayTagContainer& CreatureCreatureTypes) const
{
	return ((CreatureLevel >= LevelRequirement) && (CreatureBodyPartSockets.HasAllExact(RequiredSocketsTag))
				&& (CreatureCreatureTypes.HasTagExact(RequiredCreatureType)));
}


void FImageFragment::Assimilate(UCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);

	ULeaf_Image* LeafImage = Cast<ULeaf_Image>(Composite);
	if (!IsValid(LeafImage)) return;

	LeafImage->SetImage(Image);

	FVector2D DesiredImageDimensions = (bOverrideImageDimensions) ? ImageDimensions : Image->GetImportedSize();
	LeafImage->SetBoxSize(DesiredImageDimensions);
	LeafImage->SetImageSize(DesiredImageDimensions);
	
	LeafImage->SetImageTint(ImageTint);
}
