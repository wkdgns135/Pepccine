#pragma once

#include "CoreMinimal.h"
#include "Item/PepccineItemDataBase.h"
#include "Item/Passive/PepccineStatModifier.h"

#include "PepccineActiveItemData.generated.h"

class UPepccineItemManagerComponent;
class APepCharacter;

UCLASS(BlueprintType, Abstract)
class PEPCCINE_API UPepccineActiveItemData : public UPepccineItemDataBase
{
	GENERATED_BODY()

public:
	// 재사용 대기시간 가져오기
	FORCEINLINE float GetCooldown() const { return Cooldown; };

protected:
	// 재사용 대기시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "재사용 대기시간", ClampMin = "0.0"))
	float Cooldown = 0.0f;
};

UCLASS(BlueprintType)
class PEPCCINE_API UPepccinePotionItemData : public UPepccineActiveItemData
{
	GENERATED_BODY()

public:
	// 지속시간 가져오기
	UFUNCTION(BlueprintPure, Category = "Modifier")
	FORCEINLINE float GetDuration() const { return Duration; };
	// 무기 스탯 수정 목록 가져오기
	UFUNCTION(BlueprintPure, Category = "Modifier")
	FORCEINLINE TArray<FPepccineWeaponStatModifier> GetWeaponStatModifiers() const { return WeaponStatModifiers; };
	// 캐릭터 스탯 수정 목록 가져오기
	UFUNCTION(BlueprintPure, Category = "Modifier")
	FORCEINLINE TArray<FPepccineCharacterStatModifier> GetCharacterStatModifiers() const
	{
		return CharacterStatModifiers;
	};

protected:
	// 지속시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Potion",
		meta = (DisplayName = "지속시간", ClampMin = "1.0"))
	float Duration = 1.0f;

	// 무기 스탯 수정 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Potion", meta = (DisplayName = "무기 스탯 수정 목록"))
	TArray<FPepccineWeaponStatModifier> WeaponStatModifiers;

	// 캐릭터 스탯 수정 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Potion", meta = (DisplayName = "캐릭터 스탯 수정 목록"))
	TArray<FPepccineCharacterStatModifier> CharacterStatModifiers;
};
