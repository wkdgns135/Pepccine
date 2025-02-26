﻿#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PepccineItemDataAssetBase.generated.h"

class UPepccinePassiveItemData;
class UPepccineWeaponItemData;

UCLASS(BlueprintType)
class PEPCCINE_API UPepccineItemDataAssetBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// getter
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE TArray<UPepccineWeaponItemData*> GetWeaponsItems() { return WeaponItems; };
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE TArray<UPepccinePassiveItemData*> GetPassiveItems() { return PassiveItems; };

protected:
	// 전체 무기 데이터 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UPepccineWeaponItemData>> WeaponItems;

	// 전체 패시브 데이터 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UPepccinePassiveItemData>> PassiveItems;

	// TODO[명관] : UseableItemData 추가
};
