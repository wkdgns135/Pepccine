#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/Weapon/PepccineWeaponItemData.h"
#include "Item/Weapon/PepccineWeaponItemComponent.h"

#include "PepccineItemManagerComponent.generated.h"

class UPepccineItemDataAssetBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PEPCCINE_API UPepccineItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPepccineItemManagerComponent();

	// 무기 획득
	void PickUpWeaponItem(UPepccineWeaponItemData* WeaponItemData);

	// int 값처럼 사용하기 위한 소수점 아래 버림
	static FORCEINLINE void TruncateFloatStat(float& FloatStat) { FloatStat = FMath::TruncToFloat(FloatStat); };

	// getter
	FORCEINLINE TObjectPtr<UPepccineWeaponItemData> GetMainWeaponData() const { return MainWeaponData; };
	FORCEINLINE TObjectPtr<UPepccineWeaponItemData> GetSubWeaponData() const { return SubWeaponData; };
	FORCEINLINE TObjectPtr<UPepccineWeaponItemData> GetEquippedWeaponData() const
	{
		return WeaponItemComp->GetEquippedWeaponData();
	};
	FORCEINLINE UPepccineWeaponItemComponent* GetWeaponItemComp() const { return WeaponItemComp; };

protected:
	// 무기 컴포넌트
	UPROPERTY(VisibleInstanceOnly, Category = "Item")
	UPepccineWeaponItemComponent* WeaponItemComp;

	// 원본 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UPepccineItemDataAssetBase* ItemDataAsset;

	// 주 무기
	UPROPERTY(VisibleInstanceOnly, Category = "Item", meta = (DisplayName = "주 무기"))
	TObjectPtr<UPepccineWeaponItemData> MainWeaponData;
	// 보조 무기
	UPROPERTY(VisibleInstanceOnly, Category = "Item", meta = (DisplayName = "보조 무기"))
	TObjectPtr<UPepccineWeaponItemData> SubWeaponData;

	// 액티브 아이템

	// 카드키

	// 돈

	// 패시브 아이템 목록

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

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
