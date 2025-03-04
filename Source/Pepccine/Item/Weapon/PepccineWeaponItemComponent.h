#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"

#include "PepccineWeaponItemComponent.generated.h"

class UPepccinePoolSubSystem;
class APepccineProjectile;
class UPepccineItemManagerComponent;
class UWeaponStatModifier;
class UPepccineWeaponItemData;

UCLASS(BlueprintType)
class PEPCCINE_API UPepccineWeaponItemComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// 초기화
	void InitWeaponComponent(ACharacter* InOwnerCharacter);
	// 무기 발사
	void Fire(const float& WeaponDamage, const FVector& ShootDirection);
	// 무기 재장전
	bool Reload() const;
	// 무기 장착
	void EquipWeapon(UPepccineWeaponItemData* WeaponItemData, bool bIsPlayEquipSound = true);

	// 확산 각도 계산
	float CalculateSpreadAngle(float BaseAngle, float MaxAngle) const;
	
	// 발사 가능 여부
	FORCEINLINE void CanFire()
	{
		UE_LOG(LogTemp, Warning, TEXT("발사 가능!"));
		bCanFire = true;
	};

	// getter

	// 머즐 이름 가져오기
	static FORCEINLINE FName GetMuzzleName() { return FName(TEXT("Muzzle")); }

	// 머즐 위치 가져오기
	FORCEINLINE FVector GetMuzzleLocation(const FName SoketName) const { return GetSocketLocation(SoketName); }

	// 장착 무기 데이터 가져오기
	UFUNCTION(BlueprintPure, Category = "Item|Weapon")
	FORCEINLINE UPepccineWeaponItemData* GetEquippedWeaponData() const { return EquippedWeaponData; }
	UFUNCTION(BlueprintPure, Category = "Item|Weapon")
	FORCEINLINE TSubclassOf<APepccineProjectile> GetProjectileClass() const { return ProjectileClass; }

private:
	// 무기를 들고 있는 캐릭터
	UPROPERTY()
	ACharacter* OwnerCharacter;

	// 현재 장착중인 무기 데이터
	UPROPERTY()
	TObjectPtr<UPepccineWeaponItemData> EquippedWeaponData;

	// 투사체(프로젝타일) 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info|Weapon",
		meta = (DisplayName = "투사체 클래스", AllowPrivateAccess = true))
	TSubclassOf<APepccineProjectile> ProjectileClass;

	// 발사 가능 여부
	bool bCanFire = true;

	// 오브젝트 풀 서브 시스템
	UPROPERTY()
	UPepccinePoolSubSystem* PoolSubSystem;
};
