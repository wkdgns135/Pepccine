#include "PepccineActiveItemDataAsset.h"

#include "PepccineActiveItemData.h"

void UPepccineActiveItemDataAsset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UPepccineActiveItemDataAsset, ActiveItems))
	{
		for (int32 i = 0; i < ActiveItems.Num(); i++)
		{
			if (ActiveItems[i])
			{
				if (UPepccineActiveItemData* ActiveItem = dynamic_cast<UPepccineActiveItemData*>(ActiveItems[i]))
				{
					ActiveItem->SetItemId(i);
				}
			}
		}
	}
}
