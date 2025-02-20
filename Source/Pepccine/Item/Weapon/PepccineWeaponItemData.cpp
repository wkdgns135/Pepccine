#include "Item/Weapon/PepccineWeaponItemData.h"
#include "Item/Weapon/PepccineProjectile.h"
#include "Item/PepccineItemManagerComponent.h"

void UPepccineWeaponItemData::UseItem(UPepccineItemManagerComponent* ItemManagerComp)
{
	// 현재 탄창에 남은 탄약 수가 0이 아닐 경우
	if (WeaponStat.MagazineAmmo != 0)
	{
		Fire(ItemManagerComp);
	}
	// 현재 탄창에 남은 탄약이 없을 경우
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("탄창에 남은 탄약이 없습니다. %d / %s"), WeaponStat.MagazineAmmo, *GetSpareAmmoString());
	}
}

void UPepccineWeaponItemData::Fire(UPepccineItemManagerComponent* ItemManagerComp)
{
	if (ProjectileClass != nullptr)
	{
		if (UWorld* const World = GetWorld())
		{
			if (AActor* WeaponActor = ItemManagerComp->GetOwner())
			{
				const FRotator SpawnRotation = WeaponActor->GetActorRotation();
				const FVector SpawnLocation = WeaponActor->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// 탄약 감소
				WeaponStat.MagazineAmmo--;

				// 투사체 생성
				World->SpawnActor<APepccineProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

				UE_LOG(LogTemp, Warning, TEXT("%s 발사! %d / %s"), *DisplayName, WeaponStat.MagazineAmmo, *GetSpareAmmoString());
			}
		}
	}

	//// Try and play the sound if specified
	//if (FireSound != nullptr)
	//{
	//	UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	//}

	//// Try and play a firing animation if specified
	//if (FireAnimation != nullptr)
	//{
	//	// Get the animation object for the arms mesh
	//	UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
	//	if (AnimInstance != nullptr)
	//	{
	//		AnimInstance->Montage_Play(FireAnimation, 1.f);
	//	}
	//}
}

void UPepccineWeaponItemData::Reload()
{
	// 메인 무기의 경우
	if (WeaponItemType == EPepccineWeaponItemType::EPWIT_Main)
	{
		if (WeaponStat.SpareAmmo > 0)
		{
			int32 ModifyValue = WeaponStat.MagazineSize - WeaponStat.MagazineAmmo;

			WeaponStat.MagazineAmmo += WeaponStat.SpareAmmo >= ModifyValue ? ModifyValue : WeaponStat.SpareAmmo;
			WeaponStat.SpareAmmo = FMath::Clamp(WeaponStat.SpareAmmo - ModifyValue, 0, 999);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("남은 예비 탄약이 없습니다."));
			return;
		}
	}
	// 보조 무기의 경우
	else
	{
		WeaponStat.MagazineAmmo = WeaponStat.MagazineSize;
	}

	UE_LOG(LogTemp, Warning, TEXT("%s 재장전! %d / %s"), *DisplayName, WeaponStat.MagazineAmmo, *GetSpareAmmoString());
}
