#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PepccineItemDataAssetBase.generated.h"

class UPepccineActiveItemData;
class UPepccinePassiveItemData;
class UPepccineWeaponItemData;

UCLASS(BlueprintType)
class PEPCCINE_API UPepccineItemDataAssetBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// getter
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UStaticMesh* GetDefaultMeshToSpawn() { return DefaultMeshToSpawn; };
	
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE TArray<UPepccineWeaponItemData*> GetWeaponsItems() { return WeaponItems; };
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccineWeaponItemData* GetWeaponsItemById(const int32 Id) { return WeaponItems[Id]; };
	
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE TArray<UPepccinePassiveItemData*> GetPassiveItems() { return PassiveItems; };
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccinePassiveItemData* GetPassiveItemById(const int32 Id) { return PassiveItems[Id]; };
	
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE TArray<UPepccineActiveItemData*> GetActiveItems() { return ActiveItems; };
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccineActiveItemData* GetActiveItem(const int32 Id) { return ActiveItems[Id]; };


#if WITH_EDITOR
	template <typename T>
	void InitItemData(TArray<T*> ItemDatas);

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// 기본 스폰 메시
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "기본 스폰 메시"))
	TObjectPtr<UStaticMesh> DefaultMeshToSpawn;

	// 전체 무기 데이터 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UPepccineWeaponItemData*> WeaponItems;

	// 전체 패시브 데이터 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UPepccinePassiveItemData*> PassiveItems;

	// 액티브 아이템 데이터 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UPepccineActiveItemData*> ActiveItems;
	
	// TODO[명관] : 재화(카드키, 돈) 추가
};
