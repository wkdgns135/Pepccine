#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PepccineItemDataAssetBase.generated.h"

class UPepccineWeaponItemData;

UCLASS(BlueprintType)
class PEPCCINE_API UPepccineItemDataAssetBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 전체 무기 데이터 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UPepccineWeaponItemData>> Weapons;

	// TODO[명관] : UseableItemData 추가

};
