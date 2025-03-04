#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PepccineItemSpawnWeightData.generated.h"

class UPepccineItemRarityWeightData;

UCLASS()
class PEPCCINE_API UPepccineItemSpawnWeightData : public UDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE UPepccineItemRarityWeightData* GetItemRarityWeightData() const { return ItemRarityWeightData; }
	FORCEINLINE int32 GetWeaponItemWeight() const { return WeaponItemWeight; }
	FORCEINLINE int32 GetPassiveItemWeight() const { return PassiveItemWeight; }
	FORCEINLINE int32 GetActiveItemWeight() const { return ActiveItemWeight; }
	FORCEINLINE int32 GetAmmoBoxItemWeight() const { return AmmoBoxItemWeight; }
	FORCEINLINE int32 GetCoinItemWeight() const { return CoinItemWeight; }
	FORCEINLINE int32 GetHealingPotionItemWeight() const { return HealingPotionItemWeight; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "아이템 레어도 가중치 데이터 에셋"))
	UPepccineItemRarityWeightData* ItemRarityWeightData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "무기 아이템 가중치", ClampMin = "0"))
	int32 WeaponItemWeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "패시브 아이템 가중치", ClampMin = "0"))
	int32 PassiveItemWeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "액티브 아이템 가중치", ClampMin = "0"))
	int32 ActiveItemWeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "탄약 아이템 가중치", ClampMin = "0"))
	int32 AmmoBoxItemWeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "코인 아이템 가중치", ClampMin = "0"))
	int32 CoinItemWeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "체력 포션 아이템 가중치", ClampMin = "0"))
	int32 HealingPotionItemWeight;
};
