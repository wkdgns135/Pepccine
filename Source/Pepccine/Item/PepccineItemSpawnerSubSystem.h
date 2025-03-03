#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "PepccineItemSpawnerSubSystem.generated.h"

class UPepccineItemDataAssetBase;
class UPepccineItemDataBase;
class APepccineDropItem;

UCLASS()
class PEPCCINE_API UPepccineItemSpawnerSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// 무조건 생성
	FORCEINLINE virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }
	
	// 아이템 스포너 초기화
	UFUNCTION(BlueprintCallable, category = "Item|Spawner")
	void InitSpawner(UPepccineItemDataAssetBase* InItemDataAsset, const TSubclassOf<APepccineDropItem>& InSpawnedActor);

	// 아이템 스폰
	UFUNCTION(BlueprintCallable, category = "Item|Spawner")
	void SpawnItem(const FVector& SpawnLocation, UPepccineItemDataBase* DropItemData);
	
	// getter
	// 아이템 데이터 에셋 가져오기
	UFUNCTION(BlueprintPure, category = "Item|Spawner")
	FORCEINLINE UPepccineItemDataAssetBase* GetItemDataAsset() const { return ItemDataAsset; }
	
protected:
	// 전체 아이템 데이터 에셋
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UPepccineItemDataAssetBase* ItemDataAsset;
	
	// 스폰 엑터
	UPROPERTY(EditDefaultsOnly, Category = "Item|Spawner", DisplayName = "스폰 엑터")
	TSubclassOf<APepccineDropItem> SpawnedActor;
	
};
