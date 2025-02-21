#include "Item/Weapon/WeaponStatModifier.h"

#include "PepccineWeaponItemData.h"

float& UWeaponStatModifier::GetWeaponStatByName(EPepccineWeaponStatName StatName) const
{
	switch (StatName)
	{
	case EPepccineWeaponStatName::EPWSN_AttackMultiplier: return EquippedWeaponItemStats->AttackMultiplier;
	case EPepccineWeaponStatName::EPWSN_RangeMultiplier: return EquippedWeaponItemStats->RangeMultiplier;
	case EPepccineWeaponStatName::EPWSN_FireRateMultiplier: return EquippedWeaponItemStats->FireRateMultiplier;
	case EPepccineWeaponStatName::EPWSN_ZoomMultiplier: return EquippedWeaponItemStats->ZoomMultiplier;
	case EPepccineWeaponStatName::EPWSN_MagazineSize: return EquippedWeaponItemStats->MagazineSize;
	case EPepccineWeaponStatName::EPWSN_MagazineAmmo: return EquippedWeaponItemStats->MagazineAmmo;
	case EPepccineWeaponStatName::EPWSN_SpareAmmo: return EquippedWeaponItemStats->SpareAmmo;
	case EPepccineWeaponStatName::EPWSN_BulletSpeed: return EquippedWeaponItemStats->BulletSpeed;
	case EPepccineWeaponStatName::EPWSN_ReloadSpeed: return EquippedWeaponItemStats->ReloadSpeed;
	case EPepccineWeaponStatName::EPWSN_ProjectileCount: return EquippedWeaponItemStats->ProjectileCount;
	case EPepccineWeaponStatName::EPWSN_BulletSpread: return EquippedWeaponItemStats->BulletSpread;
	case EPepccineWeaponStatName::EPWSN_Recoil: return EquippedWeaponItemStats->Recoil;
	case EPepccineWeaponStatName::EPWSN_Weight: return EquippedWeaponItemStats->Weight;
	}
	
	UE_LOG(LogTemp,Warning,TEXT("무기 스탯 이름이 존재하지 않습니다."));
	
	return EquippedWeaponItemStats->AttackMultiplier;
}

void UWeaponStatModifier::ModifyStat(EPepccineStatModifyType StatModifyType,
                                     EPepccineWeaponStatName StatName, float Amount) const
{
	// 스탯 가져오기
	float& TargetStat = GetWeaponStatByName(StatName);
	
	// 연산 타입에 따라 연산 후 적용
	TargetStat = StatModifyType == EPepccineStatModifyType::EPSMT_Add
		             ? Additive(TargetStat, Amount)
		             : Multiplicative(TargetStat, Amount);
}

void UWeaponStatModifier::AddWeaponStatModifier(FPepccineWeaponStatModifier WeaponStatModifier)
{
	WeaponStatModifier.Id = WeaponStatModifierId;
	AppliedWeaponStatModifiers.Add(WeaponStatModifierId++, WeaponStatModifier);
}

float UWeaponStatModifier::Additive(float& Stat, float Amount)
{
	return Stat += Amount;
}

float UWeaponStatModifier::Multiplicative(float& Stat, float Amount)
{
	return Stat *= 1.0f + Amount;
}
