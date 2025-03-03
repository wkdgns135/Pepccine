#include "PepccineItemSpawnerSubSystem.h"

#include "ItemSpawnWeightData.h"
#include "PepccineDropItem.h"
#include "PepccineItemDataBase.h"

void UPepccineItemSpawnerSubSystem::InitSpawner(UPepccineItemDataAssetBase* InItemDataAsset,
	const TSubclassOf<APepccineDropItem>& InSpawnedActor)
{
	ItemDataAsset = InItemDataAsset;
	SpawnedActor = InSpawnedActor;
}

void UPepccineItemSpawnerSubSystem::SpawnItem(const FVector& SpawnLocation, UPepccineItemDataBase* DropItemData) const
{
	if (!ItemDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 데이터 에셋이 설정되지 않았습니다."));
		return;
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
			DropItem->InitializeDropItem(DropItemData);
		}
	}
}

void UPepccineItemSpawnerSubSystem::SpawnItemFromDataAsset(const FVector& SpawnLocation,
	UItemSpawnWeightData* SpawnWeightData)
{
	TArray<int32> Weights = {
		SpawnWeightData->WeaponItemWeight,
		SpawnWeightData->PassiveItemWeight,
		SpawnWeightData->ActiveItemWeight,
		SpawnWeightData->AmmoBoxItemWeight,
		SpawnWeightData->CoinItemWeight,
		SpawnWeightData->HealingPotionItemWeight
	};

	// 누적 가중치 적용
	for (int32 i = 1; i < Weights.Num(); ++i)
	{
		Weights[i] += Weights[i - 1];
	}

	// 랜덤 값
	int32 RandomValue = FMath::RandRange(0, Weights.Last() - 1);
	
}
