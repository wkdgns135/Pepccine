#include "Item/Passive/PepccinePassiveItemData.h"

bool UPepccinePassiveItemData::HasCharacterFeaturesByName(const EPepccineCharacterFeatureName CharacterFeatureName)
{
	for (FPepccineCharacterFeature CharacterFeature : CharacterFeatures)
	{
		if (CharacterFeatureName == CharacterFeature.CharacterFeatureName)
		{
			return true;
		}
	}
	return false;
}
