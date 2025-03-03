#include "PepccineWeaponItemDataAsset.h"

#include "PepccineWeaponItemData.h"

void UPepccineWeaponItemDataAsset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UPepccineWeaponItemDataAsset, WeaponItems))
	{
		for (int32 i = 0; i < WeaponItems.Num(); i++)
		{
			if (WeaponItems[i])
			{
				if (UPepccineWeaponItemData* WeaponItem = dynamic_cast<UPepccineWeaponItemData*>(WeaponItems[i]))
				{
					WeaponItem->SetItemId(i);
				}
			}
		}
	}
}
