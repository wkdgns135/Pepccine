#pragma once

#include "CoreMinimal.h"
#include "Item/PepccineItemDataBase.h"
#include "Item/Weapon/PepccineWeaponStat.h"

#include "PepccineWeaponItemData.generated.h"

class UPepccineItemManagerComponent;

UENUM(BlueprintType)
enum class EPepccineWeaponItemType : uint8
{
	EPWIT_Main UMETA(DisplayName = "주 무기"),
	EPWIT_Sub UMETA(DisplayName = "보조 무기")
};

UCLASS(BlueprintType)
class PEPCCINE_API UPepccineWeaponItemData : public UPepccineItemDataBase
{
	GENERATED_BODY()
	
public:
	// 무기 타입(주 무기, 보조 무기)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Weapon", meta = (DisplayName = "무기 타입"))
	EPepccineWeaponItemType WeaponItemType;

	// 장착시 보여지는 메시
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Weapon", meta = (DisplayName = "장착 메시"))
	TObjectPtr<USkeletalMesh> EquippedMesh;

	// 총구 오프셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Weapon", meta = (DisplayName = "총구 오프셋"))
	FVector MuzzleOffset;

	// 무기 스텟
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Weapon", meta = (DisplayName = "무기 스텟"))
	FPepccineWeaponStat WeaponStat;

	// 투사체(프로젝타일) 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Weapon", meta = (DisplayName = "투사체 클래스"))
	TSubclassOf<class APepccineProjectile> ProjectileClass;

	// 무기 발사 소리
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	//USoundBase* FireSound;

	// 무기 발사 애니메이션 몽타주
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	//UAnimMontage* FireAnimation;

	
	// 아이템 사용
	virtual void UseItem(UPepccineItemManagerComponent* ItemManagerComp) override;

	// 무기 발사
	void Fire(UPepccineItemManagerComponent* ItemManagerComp);
	
	// 무기 재장전
	UFUNCTION(BlueprintCallable)
	void Reload();

	// 장착 메시 반환
	FORCEINLINE TObjectPtr<USkeletalMesh> GetEquippedMesh() { return EquippedMesh; };

	// 디버그용 예비 탄약 수
	FORCEINLINE FString GetSpareAmmoString() { return WeaponItemType == EPepccineWeaponItemType::EPWIT_Main ? FString::FromInt(WeaponStat.SpareAmmo) : "INF"; };
};
