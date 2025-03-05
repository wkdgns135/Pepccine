#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PepccineItemQualityWeightData.generated.h"

UCLASS()
class PEPCCINE_API UPepccineItemRarityWeightData : public UDataAsset
{
	GENERATED_BODY()

public:
	// getter
	FORCEINLINE int32 GetRarity0Weight() const { return Rarity0; }
	FORCEINLINE int32 GetRarity1Weight() const { return Rarity1; }
	FORCEINLINE int32 GetRarity2Weight() const { return Rarity2; }
	FORCEINLINE int32 GetRarity3Weight() const { return Rarity3; }
	FORCEINLINE int32 GetRarity4Weight() const { return Rarity4; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "레어도 0 가중치", ClampMin = "0"))
	int32 Rarity0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "레어도 1 가중치", ClampMin = "0"))
	int32 Rarity1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "레어도 2 가중치", ClampMin = "0"))
	int32 Rarity2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "레어도 3 가중치", ClampMin = "0"))
	int32 Rarity3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "레어도 4 가중치", ClampMin = "0"))
	int32 Rarity4;
};
