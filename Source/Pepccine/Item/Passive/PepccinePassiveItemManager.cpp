#include "PepccinePassiveItemManager.h"

#include "Item/Manager/PepccineItemManagerComponent.h"

void UPepccinePassiveItemManager::PickUpItem(const UPepccinePassiveItemData* PassiveItemData)
{
	// 복사해서 사용
	if (UPepccinePassiveItemData* NewPassiveItemData = DuplicateObject<UPepccinePassiveItemData>(PassiveItemData, this))
	{
		AddPassiveItemData(NewPassiveItemData);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("패시브 아이템 데이터가 없습니다."));
	}
}

void UPepccinePassiveItemManager::AddPassiveItemData(UPepccinePassiveItemData* InPassiveItemData)
{
	// 무기 스탯
	ItemManager->IncreaseStatsOperations(InPassiveItemData->GetWeaponStatModifiers());
	// 캐릭터 스탯
	ItemManager->IncreaseStatsOperations(InPassiveItemData->GetCharacterStatModifiers());

	UE_LOG(LogTemp, Warning, TEXT("%s 추가"), *InPassiveItemData->GetDisplayName());

	PassiveItemDatas.Add(InPassiveItemData->GetItemId(), InPassiveItemData);
}

void UPepccinePassiveItemManager::RemovePassiveItemDataById(const int32 ItemId)
{
	const UPepccinePassiveItemData* PassiveItemData = GetPassiveItemById(ItemId);

	ItemManager->DecreaseStatsOperations(PassiveItemData->GetWeaponStatModifiers());
	ItemManager->DecreaseStatsOperations(PassiveItemData->GetCharacterStatModifiers());

	PassiveItemDatas.Remove(ItemId);
}
