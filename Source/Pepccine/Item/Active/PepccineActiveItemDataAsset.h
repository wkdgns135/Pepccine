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
	// 액티브
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE TArray<UPepccineActiveItemData*> GetActiveItems() { return ActiveItems; };
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccineActiveItemData* GetActiveItem(const int32 Id) { return ActiveItems[Id]; };
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
protected:
	// 액티브 아이템 데이터 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "액티브 아이템 목록"))
	TArray<UPepccineActiveItemData*> ActiveItems;
};
