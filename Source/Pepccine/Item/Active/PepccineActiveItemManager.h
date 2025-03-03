#pragma once

#include "CoreMinimal.h"
#include "Item/PepccineItemSubManager.h"
#include "UObject/Object.h"
#include "PepccineActiveItemManager.generated.h"

class UPepccineItemManagerComponent;
class UPepccinePotionItemData;
class UPepccineActiveItemData;

UCLASS()
class PEPCCINE_API UPepccineActiveItemManager : public UPepccineItemSubManager
{
	GENERATED_BODY()

public:
	// 액티브 아이템 획득
	void PickUpItem(const UPepccineActiveItemData* InActiveItemData);
	// 액티브 아이템 사용
	void UseActiveItem();
	// 포션 사용
	void ActivatePotionItem(const UPepccinePotionItemData* PotionItemData) const;
	// 버프 포션 효과 제거
	UFUNCTION()
	void DeactivatePotionItem(const UPepccinePotionItemData* PotionItemData) const;

	// 아이디로 적용된 버프 포션 목록에서 찾기
	UPepccinePotionItemData* GetAppliedPotionItemDataById(const int32 Id) const;
	
	// getter
	// 액티브 아이템 가져오기
	FORCEINLINE UPepccineActiveItemData* GetActiveItemData() const { return ActiveItemData; }
	// 적용된 버프 포션 목록 가져오기
	FORCEINLINE TArray<UPepccinePotionItemData*> GetAppliedBuffPotionItemDatas() const
	{
		return AppliedBuffPotionItemDatas;
	}

	// 현재 남은 재사용 대기시간 가져오기
	FORCEINLINE float GetActiveItemRemainingCooldown() const { return ActiveItemRemainingCooldown; }
	// 현재 재사용 대기 중 인지 확인
	FORCEINLINE bool IsActiveItemCooldown() const { return bIsActiveItemCooldown; }

	// setter
	// 액티브 아이템 남은 재사용 대기시간 설정
	FORCEINLINE void SetActiveItemRemainingCooldown(const float RemainingCooldown)
	{
		ActiveItemRemainingCooldown = RemainingCooldown;
	}

	// 액티브 아이템 재사용 대기 중 상태 설정
	FORCEINLINE void SetIsActiveItemCooldown(const bool bInIsActiveItemCooldown)
	{
		bIsActiveItemCooldown = bInIsActiveItemCooldown;
	}

private:
	// 액티브 아이템
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Active", meta = (DisplayName = "액티브 아이템"))
	TObjectPtr<UPepccineActiveItemData> ActiveItemData;

	// 적용된 버프 포션 목록
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Active", meta = (DisplayName = "적용된 버프 포션 목록"))
	TArray<TObjectPtr<UPepccinePotionItemData>> AppliedBuffPotionItemDatas;

	// 액티브 아이템 재사용 대기시간
	float ActiveItemRemainingCooldown = 0.0f;
	// 액티브 아이템 재사용 대기 중 상태
	bool bIsActiveItemCooldown = true;
};
