#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PepccineWeaponItemDataAsset.generated.h"

class UPepccineWeaponItemData;

UCLASS(BlueprintType)
class PEPCCINE_API UPepccineWeaponItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 무기
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE TArray<UPepccineWeaponItemData*> GetWeaponItemDatas() { return WeaponItems; };
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccineWeaponItemData* GetWeaponsItemById(const int32 Id) { return WeaponItems[Id]; };

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
protected:
	
	// 전체 무기 데이터 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "무기 아이템 목록"))
	TArray<UPepccineWeaponItemData*> WeaponItems;
};
