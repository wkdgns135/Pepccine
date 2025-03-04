#include "PepccineActiveItemDataAsset.h"

#include "PepccineActiveItemData.h"

void UPepccineActiveItemDataAsset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UPepccineActiveItemDataAsset, ActiveItemDatas))
	{
		for (int32 i = 0; i < ActiveItemDatas.Num(); i++)
		{
			if (ActiveItemDatas[i])
			{
				if (UPepccineActiveItemData* ActiveItem = dynamic_cast<UPepccineActiveItemData*>(ActiveItemDatas[i]))
				{
					ActiveItem->SetItemId(i);
				}
			}
		}
	}
}
