#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PepccineWeaponItemDataAsset.generated.h"

class UPepccineItemDataBase;
class UPepccineWeaponItemData;

UCLASS(BlueprintType)
class PEPCCINE_API UPepccineWeaponItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// getter
	
	// 무기 데이터 목록 가져오기
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE TArray<UPepccineWeaponItemData*> GetWeaponItemDatas() { return WeaponItemDats; }
	// 아이템 아이디로 무기 데이터 가져오기
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccineWeaponItemData* GetWeaponItemDatasById(const int32 Id) { return WeaponItemDats[Id]; }

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
protected:
	
	// 전체 무기 데이터 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "무기 아이템 목록"))
	TArray<UPepccineWeaponItemData*> WeaponItemDats;
};
