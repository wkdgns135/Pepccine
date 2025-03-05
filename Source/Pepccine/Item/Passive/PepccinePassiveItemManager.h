#pragma once

#include "CoreMinimal.h"
#include "PepccinePassiveItemData.h"
#include "Item/Manager/PepccineItemSubManager.h"
#include "UObject/Object.h"
#include "PepccinePassiveItemManager.generated.h"

class UPepccineItemManagerComponent;

UCLASS(BlueprintType)
class PEPCCINE_API UPepccinePassiveItemManager : public UPepccineItemSubManager
{
	GENERATED_BODY()

public:
	// 패시브 획득
	void PickUpItem(const UPepccinePassiveItemData* PassiveItemData);
	// 패시브 아이템 추가
	void AddPassiveItemData(UPepccinePassiveItemData* InPassiveItemData);
	// 패시브 아이템 제거
	void RemovePassiveItemDataById(const int32 ItemId);
	
	// getter
	// 전체 패시브 데이터 가져오기
	FORCEINLINE TMap<int32, UPepccinePassiveItemData*> GetPassiveItemDatas() { return PassiveItemDatas; };
	// 아이템 아이디로 패시브 데이터 가져오기
	FORCEINLINE UPepccinePassiveItemData* GetPassiveItemById(const int32 ItemId) { return PassiveItemDatas[ItemId]; };

private:
	// 패시브 아이템 목록
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Passive", meta = (DisplayName = "패시브 아이템 목록"))
	TMap<int32, UPepccinePassiveItemData*> PassiveItemDatas;
};
