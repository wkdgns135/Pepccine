#include "Item/Passive/PepccinePassiveItemData.h"

int UPepccinePassiveItemData::FindStatModifierByName(const EPepccineWeaponStatName WeaponStatName)
{
	for (int32 i = 0; i < WeaponStatModifiers.Num(); i++)
	{
		if (WeaponStatModifiers[i].WeaponItemStatName == WeaponStatName)
		{
			return i;
		}
	}

	return -1;
}

int UPepccinePassiveItemData::FindStatModifierByName(const EPepccineCharacterStatName CharacterStatName)
{
	for (int32 i = 0; i < CharacterStatModifiers.Num(); i++)
	{
		if (CharacterStatModifiers[i].CharacterStatName == CharacterStatName)
		{
			return i;
		}
	}

	return -1;
}

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
