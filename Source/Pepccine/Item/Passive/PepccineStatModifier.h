#pragma once

#include "CoreMinimal.h"
#include "Item/PepccineStatName.h"
#include "Item/Weapon/PepccineWeaponStat.h"

#include "PepccineStatModifier.generated.h"

UENUM(BlueprintType)
enum class EPepccineStatModifyType : uint8
{
	EPSMT_Add UMETA(DisplayName = "합연산"),
	EPSMT_Multiply UMETA(DisplayName = "곱연산")
};

// 수정자 공통
USTRUCT(BlueprintType)
struct FPepccineStatModifier
{
	GENERATED_BODY()
	
	// 수정자 아이디
	int32 Id;

	// 연산 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier",
		meta = (DisplayName = "연산 타입"))
	EPepccineStatModifyType StatModifyType;

	// 수정 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier",
		meta = (DisplayName = "수정 값"))
	float StatModifyValue;

	FPepccineStatModifier(): Id(0), StatModifyType(EPepccineStatModifyType::EPSMT_Add), StatModifyValue(0.0f)
	{
	}
};

// 무기 스탯 수정자
USTRUCT(BlueprintType)
struct FPepccineWeaponStatModifier
{
	GENERATED_BODY()
	
	// 무기 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier",
		meta = (DisplayName = "무기 타입"))
	EPepccineWeaponItemType WeaponItemType;
	// 무기 스탯 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier",
		meta = (DisplayName = "무기 스탯 이름"))
	EPepccineWeaponStatName WeaponItemStatName;

	// 수정자 공통
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Modifier")
	FPepccineStatModifier StatModifierDefault;

	// 기본 생성자
	FPepccineWeaponStatModifier(): WeaponItemType(EPepccineWeaponItemType::EPWIT_Main),
								   WeaponItemStatName(EPepccineWeaponStatName::EPWSN_AttackMultiplier)
	{
	}
};

// 캐릭터 스탯 수정자
USTRUCT(BlueprintType)
struct FPepccineCharacterStatModifier
{
	GENERATED_BODY()
	
	// 캐릭터 스탯 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier",
		meta = (DisplayName = "캐릭터 스탯 이름"))
	EPepccineCharacterStatName CharacterStatName;
	
	// 수정자 공통
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Modifier")
	FPepccineStatModifier StatModifierDefault;

	FPepccineCharacterStatModifier(): CharacterStatName()
	{
	}
};
