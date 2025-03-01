#pragma once

#include "CoreMinimal.h"
#include "PepccineItemSpawner.h"
#include "PepccineStatName.h"
#include "Active/PepccineActiveItemData.h"
#include "Components/ActorComponent.h"
#include "Item/Weapon/PepccineWeaponItemData.h"
#include "Item/Weapon/PepccineWeaponItemComponent.h"
#include "Weapon/PepccineWeaponItemManager.h"

#include "PepccineItemManagerComponent.generated.h"

class UPepccineActiveItemData;
struct FPepccineItemSaveData;
class UPepccinePassiveItemData;
class UPepccineItemSpawner;
class UPepccineItemDataAssetBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PEPCCINE_API UPepccineItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPepccineItemManagerComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	float GetWeaponStatByName(EPepccineWeaponItemType WeaponType, EPepccineWeaponStatName WeaponStatName) const;

	// 아이템 획득
	bool PickUpItem(UPepccineItemDataBase* DropItemData, bool bIsPlayPickUpSound = true);
	// 패시브 획득
	void PickUpItem(const UPepccinePassiveItemData* PassiveItemData);
	// 액티브 아이템 획득
	void PickUpItem(const UPepccineActiveItemData* ActiveItemData);

	// 패시브 아이템 제거
	void RemovePassiveItemDataById(const int32 ItemId);

	//////////////////////////////////////////////////////////////////////////
	// 데이터 저장 관련

	// // 아이템 세이브 데이터 구조체 가져오기
	// UFUNCTION(BlueprintCallable)
	// FPepccineItemSaveData GetSaveItemData() const;
	//
	// // 아이템 세이브 데이터 불러오기
	// UFUNCTION(BlueprintCallable)
	// void LoadItemData(const FPepccineItemSaveData& SaveData);
	//////////////////////////////////////////////////////////////////////////

	// 무기 교체
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void SwapWeapon(EPepccineWeaponItemType WeaponType) const;

	// 현재 장착 중인 무기 발사
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void FireWeapon(float WeaponDamage) const;

	// 현재 장착 중인 무기 재장전
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void ReloadWeapon() const;

	// 무기 스탯 합연산 및 곱연산 결과 값 가져오기
	UFUNCTION(BlueprintPure, Category = "Item|Weapon")
	float GetCalculatedWeaponItemStat(const EPepccineWeaponItemType WeaponItemType,
	                                  const EPepccineWeaponStatName WeaponItemStatName);

	// 무기 스탯 합연산 및 곱연산 증가
	void IncreaseStatsOperations(TArray<FPepccineWeaponStatModifier> Modifiers);
	// 캐릭터 스탯 합연산 및 곱연산 증가
	void IncreaseStatsOperations(TArray<FPepccineCharacterStatModifier> Modifiers);

	// 무기 스탯 합연산 및 곱연산 감소
	void DecreaseStatsOperations(TArray<FPepccineWeaponStatModifier> Modifiers);
	// 캐릭터 스탯 합연산 및 곱연산 감소
	void DecreaseStatsOperations(TArray<FPepccineCharacterStatModifier> Modifiers);

	// 액티브 아이템 사용
	void UseActiveItem();

	// 포션 사용
	void ActivatePotionItem(const UPepccinePotionItemData* PotionItemData);

	// 버프 포션 효과 제거
	UFUNCTION()
	void DeactivatePotionItem(const UPepccinePotionItemData* PotionItemData);

	// 아이디로 적용된 버프 포션 목록에서 찾기
	UFUNCTION(BlueprintPure, Category = "Item|Active")
	UPepccinePotionItemData* GetAppliedPotionItemDataById(const int32 Id) const;

	// // 카드키 사용
	// UFUNCTION(BlueprintCallable, Category = "Item|Resource")
	// bool UseCardKey(int32 Count);
	// 코인 사용
	UFUNCTION(BlueprintCallable, Category = "Item|Resource")
	bool UseCoin(int32 Count);

	// getter

	// 아이템 스포너 가져오기
	UFUNCTION(BlueprintCallable, Category = "Item|Spawner")
	UPepccineItemSpawner* GetItemSpawner() const { return ItemSpawner; };

	// inline getter

	// 무기 컴포넌트 가져오기
	UFUNCTION(BlueprintPure, Category = "Item|Weapon")
	FORCEINLINE UPepccineWeaponItemComponent* GetWeaponItemComp() const
	{
		return WeaponItemManager->GetWeaponItemComp();
	};
	// 무기 데이터 가져오기
	UFUNCTION(BlueprintPure, Category = "Item|Weapon")
	FORCEINLINE UPepccineWeaponItemData* GetWeaponItemData(
		const EPepccineWeaponItemType WeaponItemType) const
	{
		return WeaponItemManager->GetWeaponItemData(WeaponItemType);
	}

	// 장착 중인 무기 데이터 가져오기
	UFUNCTION(BlueprintPure, Category = "Item|Weapon")
	FORCEINLINE UPepccineWeaponItemData* GetEquippedWeaponItemData() const
	{
		return WeaponItemManager->GetEquippedWeaponItemData();
	}

	// 현재 장착중인 무기가 메인 무기인지 확인
	FORCEINLINE bool IsMainWeaponEquipped() const
	{
		return GetEquippedWeaponItemData()->GetWeaponItemType() == EPepccineWeaponItemType::EPWIT_Sub;
	};

	// 전체 패시브 데이터 가져오기
	UFUNCTION(BlueprintPure, Category = "Item|Passive")
	FORCEINLINE TMap<int32, UPepccinePassiveItemData*> GetPassiveItemDatas() { return PassiveItemDatas; };
	// 아이템 아이디로 패시브 데이터 가져오기
	UFUNCTION(BlueprintPure, Category = "Item|Passive")
	FORCEINLINE UPepccinePassiveItemData* GetPassiveItemById(const int32 ItemId) { return PassiveItemDatas[ItemId]; };

	// 스탯 이름으로 무기 스탯 합연산 총합 가져오기
	FORCEINLINE float GetTotalSumByWeaponItemStatName(const EPepccineWeaponStatName WeaponItemStatName)
	{
		return TotalWeaponStatSum.FindOrAdd({GetEquippedWeaponItemData()->GetWeaponItemType(), WeaponItemStatName});
	};
	// 스탯 이름으로 무기 스탯 곱연산 총합 가져오기
	FORCEINLINE float GetTotalProductByWeaponItemStatName(const EPepccineWeaponStatName WeaponItemStatName)
	{
		return TotalWeaponStatProduct.FindOrAdd({GetEquippedWeaponItemData()->GetWeaponItemType(), WeaponItemStatName},
		                                        1.0f);
	};

	// 스탯 이름으로 캐릭터 스탯 합연산 총합 가져오기
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE float GetTotalSumByCharacterStatName(const EPepccineCharacterStatName CharacterStatName)
	{
		return TotalCharacterStatSum.FindOrAdd(CharacterStatName);
	};
	// 스탯 이름으로 캐릭터 스탯 곱연산 총합 가져오기
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE float GetTotalProductByCharacterStatName(const EPepccineCharacterStatName CharacterStatName)
	{
		return TotalCharacterStatProduct.FindOrAdd(CharacterStatName, 1.0f);
	};

	// 액티브 아이템 가져오기
	UFUNCTION(BlueprintPure, Category = "Item|Active")
	FORCEINLINE UPepccineActiveItemData* GetActiveItemData() const { return EquippedActiveItemData; };
	// 적용된 버프 포션 목록 가져오기
	UFUNCTION(BlueprintPure, Category = "Item|Active")
	FORCEINLINE TArray<UPepccinePotionItemData*> GetAppliedBuffPotionItemDatas() const
	{
		return AppliedBuffPotionItemDatas;
	};

	// 현재 남은 재사용 대기시간 가져오기
	UFUNCTION(BlueprintCallable, Category = "Item|Active")
	FORCEINLINE float GetActiveItemRemainingCooldown() const { return ActiveItemRemainingCooldown; };

	// 현재 재사용 대기 중 인지 확인
	UFUNCTION(BlueprintCallable, Category = "Item|Active")
	FORCEINLINE bool IsActiveItemCooldown() const { return bIsActiveItemCooldown; };

	UFUNCTION(BlueprintCallable, Category = "Item|Resource")
	FORCEINLINE int32 GetCoinCount() const { return CoinCount; };

private:
	// 아이템 스포너 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPepccineItemSpawner> ItemSpawnerClass;

	// 임시 아이템 스포너
	UPROPERTY()
	UPepccineItemSpawner* ItemSpawner;

	// 무기 아이템 매니저
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Manager")
	UPepccineWeaponItemManager* WeaponItemManager;

	// // 무기 컴포넌트
	// UPROPERTY(VisibleInstanceOnly, Category = "Item|Weapon")
	// UPepccineWeaponItemComponent* WeaponItemComp;
	//
	// // 원본 주 무기 데이터
	// UPROPERTY(VisibleInstanceOnly, Category = "Item|Weapon", meta = (DisplayName = "원본 주 무기 데이터"))
	// TObjectPtr<UPepccineWeaponItemData> MainWeaponItemData;
	// // 원본 보조 무기 데이터
	// UPROPERTY(VisibleInstanceOnly, Category = "Item|Weapon", meta = (DisplayName = "원본 보조 무기 데이터"))
	// TObjectPtr<UPepccineWeaponItemData> SubWeaponItemData;

	// 패시브 아이템 목록
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Passive", meta = (DisplayName = "패시브 아이템 목록"))
	TMap<int32, UPepccinePassiveItemData*> PassiveItemDatas;

	// 무기 스탯 합연산 총합
	TMap<TPair<EPepccineWeaponItemType, EPepccineWeaponStatName>, float> TotalWeaponStatSum;
	// 무기 스탯 곱연산 총합
	TMap<TPair<EPepccineWeaponItemType, EPepccineWeaponStatName>, float> TotalWeaponStatProduct;

	// 캐릭터 스탯 합연산 총합
	TMap<EPepccineCharacterStatName, float> TotalCharacterStatSum;
	// 캐릭터 스탯 곱연산 총합
	TMap<EPepccineCharacterStatName, float> TotalCharacterStatProduct;

	// 액티브 아이템
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Active", meta = (DisplayName = "액티브 아이템"))
	TObjectPtr<UPepccineActiveItemData> EquippedActiveItemData;

	// 적용된 버프 포션 목록
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Active", meta = (DisplayName = "적용된 버프 포션 목록"))
	TArray<TObjectPtr<UPepccinePotionItemData>> AppliedBuffPotionItemDatas;

	// 액티브 아이템 재사용 대기시간
	float ActiveItemRemainingCooldown = 0.0f;
	// 액티브 아이템 재사용 대기 중 상태
	bool bIsActiveItemCooldown = true;

	// 코인
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Resource", meta = (DisplayName = "코인 수"))
	int32 CoinCount = 0;
};
