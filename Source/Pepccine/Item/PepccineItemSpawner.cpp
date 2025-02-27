#include "PepccineItemSpawner.h"
#include "PepccineDropItem.h"

void UPepccineItemSpawner::SpawnItem(const FVector& SpawnLocation, UPepccineItemDataBase* DropItemData)
{
	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		// 스폰 지점에 충돌이 있다면 위치를 조정하여 스폰
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		// 아이템 스폰
		if (APepccineDropItem* DropItem = World->SpawnActor<APepccineDropItem>(SpawnedActor, SpawnLocation, FRotator::ZeroRotator, SpawnParams))
		{
			// 아이템 초기화
			DropItem->InitializeDropItem(DropItemData);
		}
	}
}
