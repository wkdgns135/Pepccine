#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PepccinePassiveItemDataAsset.generated.h"

class UPepccinePassiveItemData;

UCLASS()
class PEPCCINE_API UPepccinePassiveItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// getter
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UStaticMesh* GetDefaultMeshToSpawn() { return DefaultMeshToSpawn; };
	
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE TArray<UPepccinePassiveItemData*> GetPassiveItems() { return PassiveItems; };
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccinePassiveItemData* GetPassiveItemById(const int32 Id) { return PassiveItems[Id]; };

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
protected:
	// 기본 스폰 메시
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "기본 스폰 메시"))
	TObjectPtr<UStaticMesh> DefaultMeshToSpawn;
	
	// 전체 패시브 데이터 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "패시브 아이템 목록"))
	TArray<UPepccinePassiveItemData*> PassiveItems;
};
