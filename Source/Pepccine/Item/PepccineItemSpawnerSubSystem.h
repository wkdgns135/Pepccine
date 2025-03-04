#pragma once

#include "CoreMinimal.h"
#include "PepccineItemQualityWeightData.h"
#include "Subsystems/WorldSubsystem.h"

#include "PepccineItemSpawnerSubSystem.generated.h"

class UPepccineItemSpawnWeightData;
class UPepccineItemDataAssetBase;
class UPepccineItemDataBase;
class APepccineDropItem;

UCLASS()
class PEPCCINE_API UPepccineItemSpawnerSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// 서브 시스템 무조건 생성
	FORCEINLINE virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

	// 누적 가중치 배열 반환
	static void ApplyCumulativeWeights(TArray<int32>& Weights);
	// 해당 등급 아이템 목록 반환
	static TArray<UPepccineItemDataBase*> GetItemDataByItemRarity(TArray<UPepccineItemDataBase*> ItemDatas,
	                                                              int32 ItemRarity);
	// 랜덤 등급 아이템 랜덤 반환
	static UPepccineItemDataBase* GetRandomItemDataByRandomRarity(const TArray<UPepccineItemDataBase*>& ItemDatas,
	                                                              const UPepccineItemRarityWeightData*
	                                                              RarityWeightData);

	// 아이템 스포너 초기화
	UFUNCTION(BlueprintCallable, Category = "Item|Spawner")
	void InitSpawner(UPepccineItemDataAssetBase* InItemDataAsset, const TSubclassOf<APepccineDropItem>& InSpawnedActor);
	// 아이템 스폰
	UFUNCTION(BlueprintCallable, Category = "Item|Spawner")
	void SpawnItem(const FVector& SpawnLocation, const UPepccineItemDataBase* DropItemData);
	// 확률로 아이템 스폰
	UFUNCTION(BlueprintPure, Category = "Item|Spawner")
	UPepccineItemDataBase* GetRandomItemFromWeightDataAsset(const UPepccineItemSpawnWeightData* SpawnWeightData) const;

	// getter
	// 아이템 데이터 에셋 가져오기
	UFUNCTION(BlueprintPure, Category = "Item|Spawner")
	FORCEINLINE UPepccineItemDataAssetBase* GetItemDataAsset() const { return ItemDataAsset; }


protected:
	// 전체 아이템 데이터 에셋
	UPROPERTY()
	UPepccineItemDataAssetBase* ItemDataAsset;
	// 스폰 엑터
	TSubclassOf<APepccineDropItem> SpawnedActor;
	// 이전에 스폰된 아이템 아이디 목록
};
