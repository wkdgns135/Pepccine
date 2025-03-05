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
}

UPepccineItemDataBase* UPepccineItemSpawnerSubSystem::SpawnItem(const FVector& SpawnLocation, UPepccineItemDataBase* DropItemData, const bool bIsShopItem)
{
	if (!ItemDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 데이터 에셋이 설정되지 않았습니다."));
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

			return DropItemData;
		}
	}

	return nullptr;
}

UPepccineItemDataBase* UPepccineItemSpawnerSubSystem::GetRandomItemFromWeightDataAsset(
	const UPepccineItemSpawnWeightData* SpawnWeightData) const
{
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
		// TODO[명관] : 탄약 랜덤으로 할지 고정값으로 할지 결정
		// 임시로 고정값 입력
		AmmoBoxData->SetResourceAmount(30);
		ItemData = AmmoBoxData;
		ItemTypeString = TEXT("탄약통");
	}
	// 코인
	else if (ItemTypeRandomValue <= ItemTypeWeights[4])
	{
		UPepccineResourceItemData* CoinData = ItemDataAsset->GetCoinItem();
		// TODO[명관] : 코인 랜덤으로 할지 고정값으로 할지 결정
		// 임시로 고정값 입력
		CoinData->SetResourceAmount(10);
		ItemData = CoinData;
		ItemTypeString = TEXT("코인");
	}
	// 회복 포션
	else
	{
		UPepccineResourceItemData* HealingPotion = ItemDataAsset->GetHealingPotion();
		// TODO[명관] : 코인 랜덤으로 할지 고정값으로 할지 결정
		// 임시로 고정값 입력
		HealingPotion->SetResourceAmount(50);
		ItemData = HealingPotion;
		ItemTypeString = TEXT("회복 포션");
	}

	UE_LOG(LogTemp, Warning, TEXT("아이템 타입 : %s"), *ItemTypeString);
	UE_LOG(LogTemp, Warning, TEXT("ItemName: %s"), *ItemData->GetDisplayName());

	return ItemData;
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
	UPepccineItemDataBase* ItemData;

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

	// 해당 등급 아이템이 없을 경우 재귀로 탐색
	if (ItemDatasByRarity.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("재귀로 재탐색"));
		ItemData = GetRandomItemDataByRandomRarity(ItemDatas, RarityWeightData);
	}
	else
	{
		// 해당 등급 아이템 랜덤 인덱스
		const int32 ItemIndex = FMath::RandRange(0, ItemDatasByRarity.Num() - 1);
		ItemData = ItemDatasByRarity[ItemIndex];
	}

	return ItemData;
}
