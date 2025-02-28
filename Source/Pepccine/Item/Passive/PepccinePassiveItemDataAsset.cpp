#include "PepccinePassiveItemDataAsset.h"

#include "PepccinePassiveItemData.h"

void UPepccinePassiveItemDataAsset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UPepccinePassiveItemDataAsset, PassiveItems))
	{
		for (int32 i = 0; i < PassiveItems.Num(); i++)
		{
			if (PassiveItems[i])
			{
				if (UPepccinePassiveItemData* PassiveItem = dynamic_cast<UPepccinePassiveItemData*>(PassiveItems[i]))
				{
					PassiveItem->SetItemId(i);
					if (!PassiveItem->GetMeshToSpawn())
					{
						PassiveItem->SetMeshToSpawn(DefaultMeshToSpawn);
					}
				}
			}
		}
	}
}
