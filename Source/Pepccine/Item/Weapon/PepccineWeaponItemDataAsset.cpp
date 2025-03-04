#include "PepccineWeaponItemDataAsset.h"

#include "PepccineWeaponItemData.h"

void UPepccineWeaponItemDataAsset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UPepccineWeaponItemDataAsset, WeaponItemDats))
	{
		for (int32 i = 0; i < WeaponItemDats.Num(); i++)
		{
			if (WeaponItemDats[i])
			{
				if (UPepccineWeaponItemData* WeaponItem = dynamic_cast<UPepccineWeaponItemData*>(WeaponItemDats[i]))
				{
					WeaponItem->SetItemId(i);
				}
			}
		}
	}
}
