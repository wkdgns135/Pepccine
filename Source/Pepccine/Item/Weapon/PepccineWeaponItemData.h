#pragma once

#include "CoreMinimal.h"
#include "Item/PepccineItemDataBase.h"
#include "Item/Weapon/PepccineWeaponStat.h"

#include "PepccineWeaponItemData.generated.h"

class UPepccineItemManagerComponent;
class APepccineProjectile;

UCLASS(BlueprintType)
class PEPCCINE_API UPepccineWeaponItemData : public UPepccineItemDataBase
{
	GENERATED_BODY()

public:
	// getter
	UFUNCTION(BlueprintPure, Category = "Item|Weapon")
	FORCEINLINE EPepccineWeaponItemType GetWeaponItemType() const { return WeaponItemType; };
	FORCEINLINE TObjectPtr<USkeletalMesh> GetEquippedMesh() const { return EquippedMesh; };
	FORCEINLINE TSubclassOf<APepccineProjectile> GetProjectileClass() const { return ProjectileClass; };
	FORCEINLINE FTimerHandle& GetFireRateTimerHandle() { return FireRateTimerHandle; };

	UFUNCTION(BlueprintPure, Category = "Item|Weapon")
	FORCEINLINE FPepccineWeaponStat GetWeaponItemStats() const { return WeaponStats; };
	FORCEINLINE FPepccineWeaponStat* GetWeaponItemStatsPointer() { return &WeaponStats; };
	FORCEINLINE TObjectPtr<USoundBase> GetFireSound() const { return FireSound; };
	FORCEINLINE TObjectPtr<USoundBase> GetReloadSound() const { return ReloadSound; };

	// setter
	// 무기 스탯 설정
	FORCEINLINE void SetWeaponStats(const FPepccineWeaponStat& InWeaponStats) { WeaponStats = InWeaponStats; };

protected:
	// 무기 타입(주 무기, 보조 무기)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Weapon", meta = (DisplayName = "무기 타입"))
	EPepccineWeaponItemType WeaponItemType;

	// 장착시 보여지는 메시
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Weapon", meta = (DisplayName = "장착 메시"))
	TObjectPtr<USkeletalMesh> EquippedMesh;

	// 투사체(프로젝타일) 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Weapon", meta = (DisplayName = "투사체 클래스"))
	TSubclassOf<APepccineProjectile> ProjectileClass;

	// 무기 발사 소리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Weapon", meta = (DisplayName = "무기 발사 소리"))
	TObjectPtr<USoundBase> FireSound;

	// 무기 재장전 소리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Weapon", meta = (DisplayName = "무기 재장전 소리"))
	TObjectPtr<USoundBase> ReloadSound;

	// 무기 스탯
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Weapon", meta = (DisplayName = "무기 스탯"))
	FPepccineWeaponStat WeaponStats;

	// 무기 재사용 대기시간 핸들러
	FTimerHandle FireRateTimerHandle;

	// 무기 발사 애니메이션 몽타주
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	//UAnimMontage* FireAnimation;
};
