#pragma once

#include "CoreMinimal.h"
#include "PepccineWeaponStat.h"
#include "Components/SkeletalMeshComponent.h"

#include "PepccineWeaponItemComponent.generated.h"

class UPepccineItemManagerComponent;
class UWeaponStatModifier;
class UPepccineWeaponItemData;

UCLASS(BlueprintType)
class PEPCCINE_API UPepccineWeaponItemComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// 무기 발사
	void Fire() const;
	// 무기 재장전
	void Reload() const;

	// 무기 장착
	void EquipWeapon(UPepccineWeaponItemData* WeaponItemData);

	// getter
	FORCEINLINE TObjectPtr<UPepccineWeaponItemData> GetEquippedWeaponData() const { return EquippedWeaponData; };
	FORCEINLINE UWeaponStatModifier* GetWeaponStatsModifier() const { return WeaponStatsModifier; };

	// setter
	FORCEINLINE void SetCharacter(ACharacter* Character) { OwnerCharacter = Character; };

private:
	// 무기를 들고 있는 캐릭터
	UPROPERTY()
	ACharacter* OwnerCharacter;
	
	// 현재 장착중인 무기 데이터
	UPROPERTY()
	TObjectPtr<UPepccineWeaponItemData> EquippedWeaponData;
	// 현재 장착중인 무기 스탯(수정자가 적용된 스탯)
	FPepccineWeaponStat* EquippedWeaponItemStats;

	// 무기 스탯 수정자
	UPROPERTY()
	UWeaponStatModifier* WeaponStatsModifier;

	// 발사 방향 가져오기
	FVector GetFireDirection(const FVector& MuzzleLocation) const;

	// 디버그용 카메라 방향으로 회전
	void RotateToCamera() const;
};
