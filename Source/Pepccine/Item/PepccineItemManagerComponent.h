#pragma once

#include "CoreMinimal.h"
#include "PepccineItemSpawner.h"
#include "Components/ActorComponent.h"
#include "Item/Weapon/PepccineWeaponItemData.h"
#include "Item/Weapon/PepccineWeaponItemComponent.h"

#include "PepccineItemManagerComponent.generated.h"

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

	// 무기 획득
	void PickUpWeaponItem(const UPepccineWeaponItemData* WeaponItemData);

	// 패시브 아이템 추가
	void AddPassiveItemData(UPepccinePassiveItemData* PassiveItemData);
	// 패시브 아이템 제거
	void RemovePassiveItemDataById(const int32 Id);

	// int 값처럼 사용하기 위한 소수점 아래 버림
	static FORCEINLINE void TruncateFloatStat(float& FloatStat) { FloatStat = FMath::TruncToFloat(FloatStat); };

	// getter
	UFUNCTION(BlueprintCallable, Category = "Item|Spawner")
	UPepccineItemSpawner* GetItemSpawner() const { return ItemSpawner; };
	FORCEINLINE UPepccineWeaponItemComponent* GetWeaponItemComp() const { return WeaponItemComp; };
	FORCEINLINE TObjectPtr<UPepccineWeaponItemData> GetMainWeaponData() const { return MainWeaponItemData; };
	FORCEINLINE TObjectPtr<UPepccineWeaponItemData> GetSubWeaponData() const { return SubWeaponItemData; };
	FORCEINLINE TObjectPtr<UPepccineWeaponItemData> GetEquippedWeaponData() const
	{
		return WeaponItemComp->GetEquippedWeaponData();
	};
	FORCEINLINE TMap<int32, TObjectPtr<UPepccinePassiveItemData>> GetPassiveItemDatas() { return PassiveItemDatas; };
	FORCEINLINE TObjectPtr<UPepccinePassiveItemData> GetPassiveItemById(const int32 Id) { return PassiveItemDatas[Id]; };

	UFUNCTION(BlueprintCallable, Category = "Item")
	UPepccineItemDataAssetBase* GetItemData() const { return ItemSpawner->GetItemDataAsset(); };
	
protected:
	// 아이템 스포너 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TSubclassOf<UPepccineItemSpawner> ItemSpawnerClass;
	
	// 임시 아이템 스포너
	UPROPERTY()
	UPepccineItemSpawner* ItemSpawner;

	// 무기 컴포넌트
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Weapon")
	UPepccineWeaponItemComponent* WeaponItemComp;

	// 주 무기 데이터 원본
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Weapon", meta = (DisplayName = "주 무기"))
	TObjectPtr<UPepccineWeaponItemData> MainWeaponItemData;
	// 보조 무기 데이터 원본
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Weapon", meta = (DisplayName = "보조 무기"))
	TObjectPtr<UPepccineWeaponItemData> SubWeaponItemData;

	// 패시브 아이템 아이디
	int32 PassiveItemId = 0;
	
	// 패시브 아이템 목록
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Passive", meta = (DisplayName = "패시브 아이템 목록"))
	TMap<int32, TObjectPtr<UPepccinePassiveItemData>> PassiveItemDatas;

	// 액티브 아이템

	// 카드키

	// 돈

	// 기본 무기 초기화
	UFUNCTION()
	void EquipDefaultWeapon();

	// 무기 장착
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void EquipWeapon(UPepccineWeaponItemData* Weapon);

	// 무기 교체
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void SwapWeapon(EPepccineWeaponItemType WeaponType);

	// 무기 메시 교체
	UFUNCTION()
	void ChangeWeaponEquippedMesh() const;

	// 현재 장착 중인 무기 발사
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void FireWeapon() const;

	// 현재 장착 중인 무기 재장전
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void ReloadWeapon() const;

private:
	// 캐릭터에 부착되어 있는 무기 액터 설정
	void SetWeaponItemComponent();
};
