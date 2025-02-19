#pragma once

#include "CoreMinimal.h"
#include "ItemDataBase.h"
#include "WeaponItemData.generated.h"

UENUM(BlueprintType)
enum class EWeaponItemType : uint8
{
	EWIT_Main UMETA(DisplayName = "주 무기"),
	EWIT_Sub UMETA(DisplayName = "보조 무기")
};

UCLASS(BlueprintType)
class PEPCCINE_API UWeaponItemData : public UItemDataBase
{
	GENERATED_BODY()
	
public:
	// 무기 타입(주 무기, 보조 무기)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "무기 타입"))
	EWeaponItemType WeaponItemType;

	// 장착시 보여지는 메시
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "장착 메시"))
	TObjectPtr<USkeletalMesh> EquippedMesh;

	// TODO[명관] : 무기 스텟 업데이트

	// 임시 스텟

	// 공격력 배율
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "공격력 배율", ClampMin = "0.1", ClampMax = "10.0"));
	float AttackMultiplier = 1.0f;

	// 한 번에 탄창에 장전 할 수 있는 탄약 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "탄창 용량", ClampMin = "1", ClampMax = "999"));
	int32 MagazineSize = 12;

	// 현재 탄창에 남아 있는 탄약 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "현재 탄약 수", ClampMin = "0", ClampMax = "999"));
	int32 MagazineAmmo = 12;

	// 현재 예비 탄약 수, 보조 무기는 무한
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "예비 탄약 수", ClampMin = "0", ClampMax = "999"));
	int32 SpareAmmo = 48;
	
	// 무기 발사
	virtual void UseItem() override;
	
	// 무기 재장전
	UFUNCTION(BlueprintCallable)
	void Reload();

	// 디버그용 예비 탄약 수
	FORCEINLINE FString GetSpareAmmoString() { return WeaponItemType == EWeaponItemType::Main ? FString::FromInt(SpareAmmo) : "INF"; };
};
