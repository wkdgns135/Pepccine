#include "WeaponItemData.h"

void UWeaponItemData::UseItem()
{
	// 현재 탄창에 남은 탄약 수가 0이 아닐 경우
	if (MagazineAmmo != 0)
	{
		MagazineAmmo--;
		UE_LOG(LogTemp, Warning, TEXT("%s 발사! %d / %s"), *DisplayName, MagazineAmmo, *GetSpareAmmoString());
	}
	// 현재 탄창에 남은 탄약이 없을 경우
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("탄창에 남은 탄약이 없습니다! %d / %s"), MagazineAmmo, *GetSpareAmmoString());
	}
}

void UWeaponItemData::Reload()
{
	// 메인 무기의 경우
	if (WeaponItemType == EWeaponItemType::EWIT_Main)
	{
		if (SpareAmmo > 0)
		{
			int32 ModifyValue = MagazineSize - MagazineAmmo;

			MagazineAmmo += SpareAmmo >= ModifyValue ? ModifyValue : SpareAmmo;
			SpareAmmo = FMath::Clamp(SpareAmmo - ModifyValue, 0, 999);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("남은 예비 탄약이 없습니다"));
			return;
		}
	}
	// 보조 무기의 경우
	else
	{
		MagazineAmmo = MagazineSize;
	}

	UE_LOG(LogTemp, Warning, TEXT("%s 재장전! %d / %s"), *DisplayName, MagazineAmmo, *GetSpareAmmoString());
}
