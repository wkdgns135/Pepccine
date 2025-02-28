#pragma once

#include "CoreMinimal.h"
#include "PepccineCharacterFeature.h"
#include "PepccineStatModifier.h"
#include "Item/PepccineItemDataBase.h"

#include "PepccinePassiveItemData.generated.h"

UCLASS(BlueprintType)
class PEPCCINE_API UPepccinePassiveItemData : public UPepccineItemDataBase
{
	GENERATED_BODY()

public:
	
	// 입력한 무기 스탯이 있는지 확인 - 있으면 해당 인덱스, 없으면 -1
	int FindStatModifierByName(EPepccineWeaponStatName WeaponStatName);
	// 입력한 캐릭터 스탯 수정자가 있는지 확인 - 있으면 해당 인덱스, 없으면 -1
	int FindStatModifierByName(EPepccineCharacterStatName CharacterStatName);
	
	// 캐릭터 기능 추가 여부 확인
	bool HasCharacterFeaturesByName(const EPepccineCharacterFeatureName CharacterFeatureName);
	
	// getter
	UFUNCTION(BlueprintPure, category = "Modifier")
	FORCEINLINE TArray<FPepccineWeaponStatModifier> GetWeaponStatModifiers() const { return WeaponStatModifiers; };
	UFUNCTION(BlueprintPure, category = "Modifier")
	FORCEINLINE TArray<FPepccineCharacterStatModifier> GetCharacterStatModifiers() const
	{
		return CharacterStatModifiers;
	};
	UFUNCTION(BlueprintPure, category = "Modifier")
	FORCEINLINE TArray<FPepccineCharacterFeature> GetCharacterFeatures() const { return CharacterFeatures; };

protected:
	// 무기 스탯 수정자 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Passive", meta = (DisplayName = "무기 스탯 수정 목록"))
	TArray<FPepccineWeaponStatModifier> WeaponStatModifiers;

	// 캐릭터 스탯 수정자 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Passive", meta = (DisplayName = "캐릭터 스탯 수정 목록"))
	TArray<FPepccineCharacterStatModifier> CharacterStatModifiers;

	// 캐릭터 추가 기능 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Passive",
		meta = (DisplayName = "캐릭터 추가 기능 목록"))
	TArray<FPepccineCharacterFeature> CharacterFeatures;
};
