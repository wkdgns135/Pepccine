#include "PepccineItemSpawnerSubSystem.h"

#include "PepccineItemSpawnWeightData.h"
#include "Item/PepccineDropItem.h"
#include "Item/PepccineItemDataAssetBase.h"
#include "Algo/MaxElement.h"
#include "Item/Active/PepccineActiveItemDataAsset.h"
#include "Item/Passive/PepccinePassiveItemDataAsset.h"
#include "Item/Resource/PepccineResourceItemData.h"
#include "Item/Weapon/PepccineWeaponItemData.h"
#include "Item/Passive/PepccinePassiveItemData.h"
#include "Item/Active/PepccineActiveItemData.h"

void UPepccineItemSpawnerSubSystem::InitSpawner(UPepccineItemDataAssetBase* InItemDataAsset,
                                                const TSubclassOf<APepccineDropItem>& InSpawnedActor)
{
	ItemDataAsset = InItemDataAsset;
	SpawnedActor = InSpawnedActor;
	InitSpawnableItemData();
}

void UPepccineItemSpawnerSubSystem::InitSpawnableItemData()
{
	// 무기
	for (const UPepccineWeaponItemData* WeaponItemData : ItemDataAsset->GetWeaponItemDataAsset()->GetWeaponItemDatas())
	{
		SpawnableWeaponItemDatas.Add(WeaponItemData->GetItemId());
	}
	// 패시브
	for (const UPepccinePassiveItemData* PassiveItemData : ItemDataAsset->GetPassiveItemDataAsset()->GetPassiveItemDatas())
	{
		SpawnablePassiveItemDataIds.Add(PassiveItemData->GetItemId());
	}
	// 액티브
	for (const UPepccineActiveItemData* ActiveItemData : ItemDataAsset->GetActiveItemDataAsset()->GetActiveItemDatas())
	{
		SpawnableActiveItemDataIds.Add(ActiveItemData->GetItemId());
	}

	UE_LOG(LogTemp, Warning, TEXT("아이템 스포너 초기화 완료!"));
}

UPepccineItemDataBase* UPepccineItemSpawnerSubSystem::SpawnItem(const FVector& SpawnLocation,
                                                                UPepccineItemDataBase* DropItemData,
                                                                const bool bIsShopItem)
{
	if (!ItemDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 데이터 에셋이 설정되지 않았습니다."));
		return nullptr;
	}

	if (!DropItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("스폰할 아이템 데이터가 없습니다."));
		return nullptr;
	}

	if (!CanSpawnItemData(DropItemData))
	{
		UE_LOG(LogTemp, Warning, TEXT("스폰 할 수 없는 아이템 입니다. : %s"), *DropItemData->GetDisplayName());
		return nullptr;
	}

	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		// 스폰 지점에 충돌이 있다면 위치를 조정하여 스폰
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// 아이템 스폰
		if (APepccineDropItem* DropItem = World->SpawnActor<APepccineDropItem>(
			SpawnedActor, SpawnLocation, FRotator::ZeroRotator, SpawnParams))
		{
			// 아이템 초기화
			DropItem->InitializeDropItem(DropItemData, bIsShopItem);
			// 스폰 가능 목록에서 제거
			RemoveSpawnableItemDataId(DropItemData);

			return DropItemData;
		}
	}

	return nullptr;
}

UPepccineItemDataBase* UPepccineItemSpawnerSubSystem::GetRandomItemFromWeightDataAsset(
	const UPepccineItemSpawnWeightData* SpawnWeightData)
{
	if (SpawnableWeaponItemDatas.IsEmpty() && SpawnablePassiveItemDataIds.
		IsEmpty() && SpawnableActiveItemDataIds.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("더이상 스폰할 무기, 패시브, 액티브 아이템이 없습니다."));
		return nullptr;
	}

	if (!SpawnWeightData)
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 스폰 가중치 데이터가 없습니다."));
		return nullptr;
	}

	// 아이템 타입 가중치
	TArray<int32> ItemTypeWeights = {
		SpawnWeightData->GetWeaponItemWeight(),
		SpawnWeightData->GetPassiveItemWeight(),
		SpawnWeightData->GetActiveItemWeight(),
		SpawnWeightData->GetAmmoBoxItemWeight(),
		SpawnWeightData->GetCoinItemWeight(),
		SpawnWeightData->GetHealingPotionItemWeight()
	};

	// 아이템 타입 누적 가중치 적용
	ApplyCumulativeWeights(ItemTypeWeights);

	// 아이템 타입 랜덤 값
	const int32 ItemTypeRandomValue = FMath::RandRange(1, *Algo::MaxElement(ItemTypeWeights));

	FString ItemTypeString;

	UPepccineItemDataBase* ItemData;
	// 무기, 패시브, 액티브 아이템에 사용
	TArray<UPepccineItemDataBase*> ItemDatas;
	// 무기 아이템
	if (ItemTypeRandomValue <= ItemTypeWeights[0])
	{
		// 무기 데이터 목록
		ItemDatas.Append(ItemDataAsset->GetWeaponItemDataAsset()->GetWeaponItemDatas());

		// 랜덤 등급으로 랜덤 아이템 가져오기
		ItemData = GetRandomItemDataByRandomRarity(ItemDatas, SpawnWeightData->GetItemRarityWeightData());
		ItemTypeString = TEXT("무기 아이템");
	}
	// 패시브 아이템
	else if (ItemTypeRandomValue <= ItemTypeWeights[1])
	{
		// 패시브 데이터 목록
		ItemDatas.Append(ItemDataAsset->GetPassiveItemDataAsset()->GetPassiveItemDatas());

		// 랜덤 등급으로 랜덤 아이템 가져오기
		ItemData = GetRandomItemDataByRandomRarity(ItemDatas, SpawnWeightData->GetItemRarityWeightData());
		ItemTypeString = TEXT("패시브 아이템");
	}
	// 액티브 아이템
	else if (ItemTypeRandomValue <= ItemTypeWeights[2])
	{
		// 액티브 데이터 목록
		ItemDatas.Append(ItemDataAsset->GetActiveItemDataAsset()->GetActiveItemDatas());

		// 랜덤 등급으로 랜덤 아이템 가져오기
		ItemData = GetRandomItemDataByRandomRarity(ItemDatas, SpawnWeightData->GetItemRarityWeightData());
		ItemTypeString = TEXT("액티브 아이템");
	}
	// 탄약통
	else if (ItemTypeRandomValue <= ItemTypeWeights[3])
	{
		UPepccineResourceItemData* AmmoBoxData = ItemDataAsset->GetAmmoBoxItem();
		ItemData = AmmoBoxData;
		ItemTypeString = TEXT("탄약통");
	}
	// 코인
	else if (ItemTypeRandomValue <= ItemTypeWeights[4])
	{
		UPepccineResourceItemData* CoinData = ItemDataAsset->GetCoinItem();
		ItemData = CoinData;
		ItemTypeString = TEXT("코인");
	}
	// 회복 포션
	else
	{
		UPepccineResourceItemData* HealingPotion = ItemDataAsset->GetHealingPotion();
		ItemData = HealingPotion;
		ItemTypeString = TEXT("회복 포션");
	}

	UE_LOG(LogTemp, Warning, TEXT("아이템 타입 : %s"), *ItemTypeString)

	// 해당 아이템이 없을 경우 재귀로 탐색
	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템이 없습니다. 재귀로 재탐색"));
		ItemData = GetRandomItemFromWeightDataAsset(SpawnWeightData);
	}

	// 해당 아이템이 스폰 가능 아이템 목록에 없을 경우
	if (!CanSpawnItemData(ItemData))
	{
		UE_LOG(LogTemp, Warning, TEXT("스폰 가능 목록에 없습니다. 재귀로 재탐색"));
		ItemData = GetRandomItemFromWeightDataAsset(SpawnWeightData);
	}

	UE_LOG(LogTemp, Warning, TEXT("ItemName: %s"), *ItemData->GetDisplayName());

	return ItemData;
}

bool UPepccineItemSpawnerSubSystem::CanSpawnItemData(const UPepccineItemDataBase* ItemData) const
{
	// 무기
	if (Cast<UPepccineWeaponItemData>(ItemData))
	{
		return SpawnableWeaponItemDatas.Contains(ItemData->GetItemId());
	}

	// 패시브
	if (Cast<UPepccinePassiveItemData>(ItemData))
	{
		return SpawnablePassiveItemDataIds.Contains(ItemData->GetItemId());
	}

	// 액티브
	if (Cast<UPepccineActiveItemData>(ItemData))
	{
		return SpawnableActiveItemDataIds.Contains(ItemData->GetItemId());
	}

	return true;
}

void UPepccineItemSpawnerSubSystem::AddSpawnableItemDataId(UPepccineItemDataBase* ItemData)
{
	// 무기
	if (Cast<UPepccineWeaponItemData>(ItemData))
	{
		SpawnableWeaponItemDatas.Add(ItemData->GetItemId());
	}
	// 패시브
	else if (Cast<UPepccinePassiveItemData>(ItemData))
	{
		SpawnablePassiveItemDataIds.Add(ItemData->GetItemId());
	}
	// 액티브
	else if (Cast<UPepccineActiveItemData>(ItemData))
	{
		SpawnableActiveItemDataIds.Add(ItemData->GetItemId());
	}
}

void UPepccineItemSpawnerSubSystem::RemoveSpawnableItemDataId(UPepccineItemDataBase* ItemData)
{
	// 무기
	if (Cast<UPepccineWeaponItemData>(ItemData))
	{
		SpawnableWeaponItemDatas.Remove(ItemData->GetItemId());
		UE_LOG(LogTemp, Warning, TEXT("스폰 가능 무기 목록에서 제거 : %s"), *ItemData->GetDisplayName());
	}
	// 패시브
	else if (Cast<UPepccinePassiveItemData>(ItemData))
	{
		SpawnablePassiveItemDataIds.Remove(ItemData->GetItemId());
		UE_LOG(LogTemp, Warning, TEXT("스폰 가능 패시브 목록에서 제거 : %s"), *ItemData->GetDisplayName());
	}
	// 액티브
	else if (Cast<UPepccineActiveItemData>(ItemData))
	{
		SpawnableActiveItemDataIds.Remove(ItemData->GetItemId());
		UE_LOG(LogTemp, Warning, TEXT("스폰 가능 액티브 목록에서 제거 : %s"), *ItemData->GetDisplayName());
	}

	UE_LOG(LogTemp, Warning, TEXT("스폰된 아이템 : %s"), *ItemData->GetDisplayName());
}

UPepccineWeaponItemData* UPepccineItemSpawnerSubSystem::GetDefaultWeaponItemData() const
{
	if (!ItemDataAsset
		|| !ItemDataAsset->GetWeaponItemDataAsset()
		|| ItemDataAsset->GetWeaponItemDataAsset()->GetWeaponItemDatas().IsEmpty())
	{
		return nullptr;
	}

	return ItemDataAsset->GetWeaponItemDataAsset()->GetWeaponItemDatasById(0);
}

void UPepccineItemSpawnerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UPepccineItemDataAssetBase* InItemDataAsset = LoadObject<UPepccineItemDataAssetBase>(nullptr, TEXT("/Script/Pepccine.PepccineItemDataAssetBase'/Game/Pepccine/Item/DA_ItemDataAsset.DA_ItemDataAsset'"));
	UClass* InSpawnedActorClass = LoadClass<APepccineDropItem>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Pepccine/Item/Bluprints/BP_DropItem.BP_DropItem_C'"));
	
	if (!InItemDataAsset || !InSpawnedActorClass)return;
	ItemDataAsset = InItemDataAsset;
	SpawnedActor = InSpawnedActorClass;

	// 스폰가능 무기 아이템 추가
	for (const UPepccineWeaponItemData* WeaponItemData : ItemDataAsset->GetWeaponItemDataAsset()->GetWeaponItemDatas())
	{
		SpawnableWeaponItemDatas.Add(WeaponItemData->GetItemId());
	}
	// 스폰가능 패시브 아이템 추가
	for (const UPepccinePassiveItemData* PassiveItemData : ItemDataAsset->GetPassiveItemDataAsset()->
																		  GetPassiveItemDatas())
	{
		SpawnablePassiveItemDataIds.Add(PassiveItemData->GetItemId());
	}
	// 스폰가능 액티브 아이템 추가
	for (const UPepccineActiveItemData* ActiveItemData : ItemDataAsset->GetActiveItemDataAsset()->GetActiveItemDatas())
	{
		SpawnableActiveItemDataIds.Add(ActiveItemData->GetItemId());
	}
}

void UPepccineItemSpawnerSubSystem::ApplyCumulativeWeights(TArray<int32>& Weights)
{
	int32 CumulativeWeights = 0;
	for (int32 i = 0; i < Weights.Num(); ++i)
	{
		if (Weights[i] > 0)
		{
			CumulativeWeights += Weights[i];
			Weights[i] = CumulativeWeights;
		}

		UE_LOG(LogTemp, Warning, TEXT("%d : %d"), i, Weights[i]);
	}
}

TArray<UPepccineItemDataBase*> UPepccineItemSpawnerSubSystem::GetItemDataByItemRarity(
	TArray<UPepccineItemDataBase*> ItemDatas, const int32 ItemRarity)
{
	TArray<UPepccineItemDataBase*> ResultItemDatas;

	for (UPepccineItemDataBase* ItemData : ItemDatas)
	{
		if (ItemData->GetItemId() == ItemRarity)
		{
			ResultItemDatas.Add(ItemData);
		}
	}

	return ResultItemDatas;
}

UPepccineItemDataBase* UPepccineItemSpawnerSubSystem::GetRandomItemDataByRandomRarity(
	const TArray<UPepccineItemDataBase*>& ItemDatas, const UPepccineItemRarityWeightData* RarityWeightData)
{
	if (!RarityWeightData)
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 등급 가중치 데이터가 없습니다."));
		return nullptr;
	}

	UPepccineItemDataBase* ItemData = nullptr;

	// 아이템 레어도 가중치
	TArray<int32> ItemRarityWeights = {
		RarityWeightData->GetRarity0Weight(),
		RarityWeightData->GetRarity1Weight(),
		RarityWeightData->GetRarity2Weight(),
		RarityWeightData->GetRarity3Weight(),
		RarityWeightData->GetRarity4Weight(),
	};

	// 아이템 레어도 가중치 적용
	ApplyCumulativeWeights(ItemRarityWeights);

	// 아이템 레어도 랜덤 값
	const int32 ItemRarityRandomValue = FMath::RandRange(1, *Algo::MaxElement(ItemRarityWeights));

	// 누적 가중치 이진 탐색
	const int32 ItemRarity = ItemRarityWeights.IndexOfByPredicate([ItemRarityRandomValue](const int32 Weight)
	{
		return Weight > ItemRarityRandomValue;
	});

	UE_LOG(LogTemp, Warning, TEXT("Item Rarity : %d"), ItemRarity);

	const TArray<UPepccineItemDataBase*> ItemDatasByRarity = GetItemDataByItemRarity(ItemDatas, ItemRarity);

	// 해당 등급 아이템이 있을 경우
	if (!ItemDatasByRarity.IsEmpty())
	{
		// 해당 등급 아이템 랜덤 인덱스
		const int32 ItemIndex = FMath::RandRange(0, ItemDatasByRarity.Num() - 1);
		ItemData = ItemDatasByRarity[ItemIndex];
	}

	return ItemData;
}
