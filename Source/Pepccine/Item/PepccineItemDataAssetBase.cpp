#include "PepccineItemDataAssetBase.h"

#include "Passive/PepccinePassiveItemData.h"
#include "Weapon/PepccineWeaponItemData.h"

#if WITH_EDITOR
template <typename T>
void UPepccineItemDataAssetBase::InitItemData(TArray<T*> ItemDatas)
{
	for (int32 i = 0; i < ItemDatas.Num(); i++)
	{
		if (ItemDatas[i])
		{
			if (UPepccineItemDataBase* ItemData = dynamic_cast<UPepccineItemDataBase*>(ItemDatas[i]))
			{
				ItemData->SetItemId(i);
				if (!ItemData->GetMeshToSpawn())
				{
					ItemData->SetMeshToSpawn(DefaultMeshToSpawn);
				}
			}
		}
	}
}

void UPepccineItemDataAssetBase::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UPepccineItemDataAssetBase, WeaponItems))
	{
		InitItemData<UPepccineWeaponItemData>(WeaponItems);
	}
	else if (PropertyChangedEvent.GetPropertyName() ==
		GET_MEMBER_NAME_CHECKED(UPepccineItemDataAssetBase, PassiveItems))
	{
		InitItemData<UPepccinePassiveItemData>(PassiveItems);
	}

	// if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UPepccineItemDataAssetBase, WeaponItems))
	// {
	// 	for (int32 i = 0; i < WeaponItems.Num(); i++)
	// 	{
	// 		if (WeaponItems[i])
	// 		{
	// 			WeaponItems[i]->SetItemId(FString::Printf(TEXT("Weapon_%d"), i));
	// 		}
	// 	}
	// }
	// else if (PropertyChangedEvent.GetPropertyName() ==
	// 	GET_MEMBER_NAME_CHECKED(UPepccineItemDataAssetBase, PassiveItems))
	// {
	// 	for (int32 i = 0; i < PassiveItems.Num(); i++)
	// 	{
	// 		if (PassiveItems[i])
	// 		{
	// 			PassiveItems[i]->SetItemId(FString::Printf(TEXT("Passive_%d"), i));
	// 		}
	// 	}
	// }
}
#endif
