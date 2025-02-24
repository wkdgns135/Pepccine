#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PepccineItemSpawner.generated.h"

class UPepccineItemDataAssetBase;
class UPepccineItemDataBase;
class APepccineDropItem;

UCLASS(Blueprintable, Abstract)
class PEPCCINE_API UPepccineItemSpawner : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, category = "Item|Spawner")
	void SpawnItem(const FVector& SpawnLocation, UPepccineItemDataBase* DropItemData);
	
	// getter
	UFUNCTION(BlueprintCallable, category = "Item|Spawner")
	FORCEINLINE UPepccineItemDataAssetBase* GetItemDataAsset() const { return ItemDataAsset; };
	
protected:
	// 전체 아이템 데이터 에셋
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UPepccineItemDataAssetBase* ItemDataAsset;
	
	// 스폰 엑터
	UPROPERTY(EditDefaultsOnly, Category = "Item|Spawner", DisplayName = "스폰 엑터")
	TSubclassOf<APepccineDropItem> SpawnedActor;
	
};
