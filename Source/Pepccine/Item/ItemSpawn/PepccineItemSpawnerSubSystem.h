#pragma once

#include "CoreMinimal.h"
#include "PepccineItemQualityWeightData.h"
#include "Item/PepccineItemDataAssetBase.h"
#include "Subsystems/WorldSubsystem.h"

#include "PepccineItemSpawnerSubSystem.generated.h"

class UPepccineItemSaveData;
class UPepccineItemSaveDataManager;
class UPepccineItemManagerComponent;
class UPepccineWeaponItemData;
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
	// 스폰가능 아이템 데이터 초기화
	void InitSpawnableItemData();
	// 아이템 스폰
	UFUNCTION(BlueprintCallable, Category = "Item|Spawner")
	UPepccineItemDataBase* SpawnItem(const FVector& SpawnLocation, UPepccineItemDataBase* DropItemData,
	                                 bool bIsShopItem = false);
	// 확률로 아이템 스폰
	UFUNCTION(BlueprintPure, Category = "Item|Spawner")
	UPepccineItemDataBase* GetRandomItemFromWeightDataAsset(const UPepccineItemSpawnWeightData* SpawnWeightData);

	// 현재 스폰할 아이템 이미 스폰된 건지 확인(Resource Item은 제외, 플레이어가 가지고 있는 아이템 포함)
	bool CanSpawnItemData(const UPepccineItemDataBase* ItemData) const;

	// 스폰가능 아이템 데이터 추가
	void AddSpawnableItemDataId(UPepccineItemDataBase* ItemData);
	// 스폰가능 아이템 데이터 제거
	void RemoveSpawnableItemDataId(UPepccineItemDataBase* ItemData);

	// getter
	// 기본 무기 데이터 가져오기
	UFUNCTION(BlueprintPure, Category = "Item|Spawner")
	UPepccineWeaponItemData* GetDefaultWeaponItemData() const;

	// 아이템 데이터 에셋 가져오기
	UFUNCTION(BlueprintPure, Category = "Item|Spawner")
	FORCEINLINE UPepccineItemDataAssetBase* GetItemDataAsset() const { return ItemDataAsset; }

protected:
	// 전체 아이템 데이터 에셋
	UPROPERTY()
	UPepccineItemDataAssetBase* ItemDataAsset;
	// 스폰 엑터
	TSubclassOf<APepccineDropItem> SpawnedActor;

	// 스폰가능한 무기 아이템 아이디 목록
	TArray<int32> SpawnableWeaponItemDatas;
	// 스폰가능한 패시브 아이템 아이디 목록
	TArray<int32> SpawnablePassiveItemDataIds;
	// 스폰가능한 액티브 아이템 아이디 목록
	TArray<int32> SpawnableActiveItemDataIds;

	// 저장 데이터
	UPROPERTY()
	UPepccineItemSaveData* SaveData;
};
