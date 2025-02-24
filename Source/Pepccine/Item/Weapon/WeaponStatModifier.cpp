#include "Item/Weapon/WeaponStatModifier.h"

#include "PepccineWeaponItemData.h"
#include "VREditorMode.h"

float& UWeaponStatModifier::GetWeaponItemStatByName(FPepccineWeaponStat* WeaponItemStats,
                                                    const EPepccineWeaponStatName StatName)
{
	switch (StatName)
	{
	case EPepccineWeaponStatName::EPWSN_AttackMultiplier: return WeaponItemStats->AttackMultiplier;
	case EPepccineWeaponStatName::EPWSN_RangeMultiplier: return WeaponItemStats->RangeMultiplier;
	case EPepccineWeaponStatName::EPWSN_FireRateMultiplier: return WeaponItemStats->FireRateMultiplier;
	case EPepccineWeaponStatName::EPWSN_ZoomMultiplier: return WeaponItemStats->ZoomMultiplier;
	case EPepccineWeaponStatName::EPWSN_MagazineSize: return WeaponItemStats->MagazineSize;
	case EPepccineWeaponStatName::EPWSN_MagazineAmmo: return WeaponItemStats->MagazineAmmo;
	case EPepccineWeaponStatName::EPWSN_SpareAmmo: return WeaponItemStats->SpareAmmo;
	case EPepccineWeaponStatName::EPWSN_BulletSpeed: return WeaponItemStats->BulletSpeed;
	case EPepccineWeaponStatName::EPWSN_ReloadSpeed: return WeaponItemStats->ReloadSpeed;
	case EPepccineWeaponStatName::EPWSN_ProjectileCount: return WeaponItemStats->ProjectileCount;
	case EPepccineWeaponStatName::EPWSN_BulletSpread: return WeaponItemStats->BulletSpread;
	case EPepccineWeaponStatName::EPWSN_Recoil: return WeaponItemStats->Recoil;
	case EPepccineWeaponStatName::EPWSN_Weight: return WeaponItemStats->Weight;
	}

	UE_LOG(LogTemp, Warning, TEXT("무기 스탯 이름이 존재하지 않습니다."));

	return WeaponItemStats->AttackMultiplier;
}

void UWeaponStatModifier::SetWeaponItemStats(const UPepccineWeaponItemData* WeaponItemData)
{
	if (WeaponItemData->GetWeaponItemType() == EPepccineWeaponItemType::EPWIT_Main)
	{
		ModifiedMainWeaponItemStats = WeaponItemData->GetWeaponItemStats();
	}
	else
	{
		ModifiedSubWeaponItemStats = WeaponItemData->GetWeaponItemStats();
	}

	ApplyAllStatModifiers(WeaponItemData->GetWeaponItemType());
}

void UWeaponStatModifier::AddWeaponStatModifier(FPepccineWeaponStatModifier WeaponStatModifier)
{
	WeaponStatModifier.Id = WeaponStatModifierId;
	AppliedMainWeaponItemStatModifiers.FindOrAdd(WeaponStatModifier.WeaponItemStatName)
	                                  .Add(WeaponStatModifierId++, WeaponStatModifier);
}

void UWeaponStatModifier::RemoveWeaponStatModifier(const FPepccineWeaponStatModifier& WeaponStatModifier)
{
	const EPepccineWeaponStatName StatName = WeaponStatModifier.WeaponItemStatName;
	const int32 Id = WeaponStatModifier.Id;

	if (WeaponStatModifier.WeaponItemType == EPepccineWeaponItemType::EPWIT_Main)
	{
		AppliedMainWeaponItemStatModifiers[StatName].Remove(Id);
	}
	else
	{
		AppliedSubWeaponItemStatModifiers[StatName].Remove(Id);
	}
}

void UWeaponStatModifier::ApplyAllStatModifiers(const EPepccineWeaponItemType WeaponItemType)
{
	// 무기 타입에 따라 수정자 목록 설정
	auto AppliedWeaponStatModifiers = WeaponItemType == EPepccineWeaponItemType::EPWIT_Main
		                                  ? AppliedMainWeaponItemStatModifiers
		                                  : AppliedSubWeaponItemStatModifiers;

	// 해당 스탯에 적용된 수정자만 가지고와서 적용
	for (const auto& [StatName, Modifiers] : AppliedWeaponStatModifiers)
	{
		for (const auto& [Id, Modifier] : Modifiers)
		{
			// 수정자 적용
			ApplyStatModifier(Modifier, false);
		}
	}
}

void UWeaponStatModifier::ApplyStatModifier(const FPepccineWeaponStatModifier WeaponItemModifier,
                                            const bool IsNewModifier)
{
	float& ModifiedStat = GetWeaponItemStatByName(
		WeaponItemModifier.WeaponItemType == EPepccineWeaponItemType::EPWIT_Main
			? &ModifiedMainWeaponItemStats
			: &ModifiedSubWeaponItemStats,
			WeaponItemModifier.WeaponItemStatName);

	const float BeforeStat = ModifiedStat;

	// 합연산
	if (WeaponItemModifier.StatModifyType == EPepccineStatModifyType::EPSMT_Add)
	{
		ModifiedStat += WeaponItemModifier.StatModifyValue;
	}
	// 곱연산
	else
	{
		ModifiedStat *= WeaponItemModifier.StatModifyValue;
	}

	// 새 수정자는 추가
	if (IsNewModifier)
	{
		AddWeaponStatModifier(WeaponItemModifier);
	}

	// 디버그용
	const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/Pepccine.EPepccineWeaponStatName"), true);
	FString StatName = "";
	if (EnumPtr)
	{
		StatName = EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(WeaponItemModifier.WeaponItemStatName)).ToString();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("실패!"));
	}
	UE_LOG(LogTemp, Warning, TEXT("수정자 적용! %s : %.2f => %.2f"), *StatName, BeforeStat, ModifiedStat);
}
