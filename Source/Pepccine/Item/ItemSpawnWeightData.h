#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemSpawnWeightData.generated.h"

UCLASS()
class PEPCCINE_API UItemSpawnWeightData : public UDataAsset
{
	GENERATED_BODY()

public:
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
