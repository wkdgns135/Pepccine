#include "PepccinePassiveItemDataAsset.h"

#include "PepccinePassiveItemData.h"

void UPepccinePassiveItemDataAsset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UPepccinePassiveItemDataAsset, PassiveItemDatas))
	{
		for (int32 i = 0; i < PassiveItemDatas.Num(); i++)
		{
			if (PassiveItemDatas[i])
			{
				if (UPepccinePassiveItemData* PassiveItem = dynamic_cast<UPepccinePassiveItemData*>(PassiveItemDatas[i]))
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
