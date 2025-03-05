#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PepccineActiveItemDataAsset.generated.h"

class UPepccineActiveItemData;

UCLASS()
class PEPCCINE_API UPepccineActiveItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// getter
	
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE TArray<UPepccineActiveItemData*> GetActiveItemDatas() { return ActiveItemDatas; };
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccineActiveItemData* GetActiveItemById(const int32 Id) { return ActiveItemDatas[Id]; };
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
protected:
	// 액티브 아이템 데이터 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "액티브 아이템 목록"))
	TArray<UPepccineActiveItemData*> ActiveItemDatas;
};
