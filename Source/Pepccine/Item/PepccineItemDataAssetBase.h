#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapon/PepccineWeaponItemDataAsset.h"
#include "PepccineItemDataAssetBase.generated.h"

class UPepccineActiveItemDataAsset;
class UPepccinePassiveItemDataAsset;
class UPepccineWeaponItemDataAsset;
class UPepccineResourceItemData;

UCLASS(BlueprintType)
class PEPCCINE_API UPepccineItemDataAssetBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// getter

	// 무기 데이터 에셋
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccineWeaponItemDataAsset* GetWeaponItemDataAsset() const { return WeaponItemDataAsset; }
	// 패시브 데이터 에셋
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccinePassiveItemDataAsset* GetPassiveItemDataAsset() const { return PassiveItemDataAsset; }
	// 액티브 데이터 에셋
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccineActiveItemDataAsset* GetActiveItemDataAsset() const { return ActiveItemDataAsset; }
	// 탄약통
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccineResourceItemData* GetAmmoBoxItem() const { return AmmoBoxItem; }
	// 코인
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccineResourceItemData* GetCoinItem() const { return CoinItem; }
	// 체력 포션
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccineResourceItemData* GetHealingPotion() const { return HealingPotion; }

protected:

	// 무기 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "무기 아이템 데이터 에셋"))
	UPepccineWeaponItemDataAsset* WeaponItemDataAsset;
	// 패시브 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "패시브 아이템 데이터 에셋"))
	UPepccinePassiveItemDataAsset* PassiveItemDataAsset;
	// 액티브 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "액티브 아이템 데이터 에셋"))
	UPepccineActiveItemDataAsset* ActiveItemDataAsset;
	// 탄약통
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "탄약통"))
	UPepccineResourceItemData* AmmoBoxItem;
	// 코인
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "코인"))
	UPepccineResourceItemData* CoinItem;
	// 체력 포션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "체력 포션"))
	UPepccineResourceItemData* HealingPotion;
};
