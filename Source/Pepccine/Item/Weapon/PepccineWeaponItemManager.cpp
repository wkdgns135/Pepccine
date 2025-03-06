#include "PepccineWeaponItemManager.h"

#include "PepccineWeaponItemData.h"
#include "GameFramework/Character.h"
#include "Item/PepccineItemDataAssetBase.h"
#include "Item/Manager/PepccineItemManagerComponent.h"
#include "Item/ItemSpawn/PepccineItemSpawnerSubSystem.h"

void UPepccineWeaponItemManager::SetWeaponItemComponent(ACharacter* OwnerCharacter)
{
	TArray<AActor*> AttachedActors;
	OwnerCharacter->GetAttachedActors(AttachedActors);

	for (const AActor* Actor : AttachedActors)
	{
		if (Actor)
		{
			if (UPepccineWeaponItemComponent* Comp = Actor->FindComponentByClass<UPepccineWeaponItemComponent>())
			{
				// 무기 컴포넌트 등록
				WeaponItemComp = Comp;

				WeaponItemComp->InitWeaponComponent(OwnerCharacter);

				UE_LOG(LogTemp, Warning, TEXT("무기 컴포넌트 설정 완료!"));
				break;
			}
		}
	}
}

void UPepccineWeaponItemManager::EquipDefaultWeapon(const UPepccineWeaponItemData* WeaponItemData)
{
	PickUpItem(WeaponItemData, false);
}

void UPepccineWeaponItemManager::PickUpItem(const UPepccineWeaponItemData* WeaponItemData, const bool bIsPlayEquipSound)
{
	if (!WeaponItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 데이터가 없습니다."));
		return;
	}
	
	// 복사해서 사용
	UPepccineWeaponItemData* NewWeaponItemData = DuplicateObject<UPepccineWeaponItemData>(
		WeaponItemData, this);
	if (NewWeaponItemData->GetWeaponItemType() == EPepccineWeaponItemType::EPWIT_Main)
	{
		MainWeaponItemData = NewWeaponItemData;
	}
	else
	{
		SubWeaponItemData = NewWeaponItemData;
	}

	// 패시브 적용하여 스탯 조정
	UpdateWeaponItemStats(NewWeaponItemData->GetWeaponItemType());

	// 획득한 무기 장착
	EquipWeapon(NewWeaponItemData, bIsPlayEquipSound);

	UE_LOG(LogTemp, Warning, TEXT("%s 등록!"), *NewWeaponItemData->GetDisplayName());
}

void UPepccineWeaponItemManager::EquipWeapon(UPepccineWeaponItemData* Weapon, const bool bIsPlayEquipSound) const
{
	// 무기가 없거나 무기 컴포넌트가 없을 경우 장착 X
	if (!Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("교체할 무기가 없습니다."));
		return;
	}

	if (!WeaponItemComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("무기 컴포넌트가 없습니다."));
		return;
	}

	// 무기 장착
	WeaponItemComp->EquipWeapon(Weapon, bIsPlayEquipSound);

	// 메시 변경
	ChangeWeaponEquippedMesh();

	UE_LOG(LogTemp, Warning, TEXT("%s 장착!"), *Weapon->GetDisplayName());
}

void UPepccineWeaponItemManager::ChangeWeaponEquippedMesh() const
{
	if (WeaponItemComp)
	{
		if (USkeletalMesh* Mesh = GetEquippedWeaponItemData()->GetEquippedMesh())
		{
			WeaponItemComp->SetSkeletalMesh(Mesh);
		}
	}
}

void UPepccineWeaponItemManager::SwapWeapon(const EPepccineWeaponItemType WeaponType) const
{
	if (GetEquippedWeaponItemData()->GetWeaponItemType() != WeaponType)
	{
		EquipWeapon(WeaponType == EPepccineWeaponItemType::EPWIT_Main
			            ? MainWeaponItemData
			            : SubWeaponItemData);
	}
}

void UPepccineWeaponItemManager::FireWeapon(const float WeaponDamage, const FVector& ShootDirection) const
{
	// 무기 컴포넌트가 있고 장착된 무기가 있을 경우만 발사
	if (WeaponItemComp && GetEquippedWeaponItemData())
	{
		WeaponItemComp->Fire(WeaponDamage, ShootDirection);
	}
}

void UPepccineWeaponItemManager::ReloadWeapon() const
{
	// 무기 컴포넌트가 있고 장착된 무기가 있을 경우만 재장전
	if (WeaponItemComp && GetEquippedWeaponItemData())
	{
		if (WeaponItemComp->Reload())
		{
			const UPepccineWeaponItemData* EquippedWeaponItemData = GetEquippedWeaponItemData();

			UE_LOG(LogTemp, Warning, TEXT("%s 재장전! %.0f / %.0f"),
			       *EquippedWeaponItemData->GetDisplayName(),
			       EquippedWeaponItemData->GetWeaponItemStats().MagazineAmmo,
			       EquippedWeaponItemData->GetWeaponItemStats().SpareAmmo);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("재장전 실패!!"));
		}
	}
}

void UPepccineWeaponItemManager::UpdateWeaponItemStats(const EPepccineWeaponItemType WeaponItemType) const
{
	TArray<UPepccinePassiveItemData*> PassiveItemDatas;
	ItemManager->GetPassiveItemDatas().GenerateValueArray(PassiveItemDatas);

	UPepccineWeaponItemData* TargetWeaponItem = WeaponItemType == EPepccineWeaponItemType::EPWIT_Main
		                                            ? MainWeaponItemData
		                                            : SubWeaponItemData;

	UPepccineWeaponItemData* DefaultWeaponItemData = GetWorld()->GetGameInstance()->
	                                                       GetSubsystem<UPepccineItemSpawnerSubSystem>()->
	                                                       GetItemDataAsset()->
	                                                       GetWeaponItemDataAsset()->
	                                                       GetWeaponItemDatasById(TargetWeaponItem->GetItemId());

	for (const UPepccinePassiveItemData* PassiveItemData : PassiveItemDatas)
	{
		for (const FPepccineWeaponStatModifier Modifier : PassiveItemData->GetWeaponStatModifiers())
		{
			const EPepccineWeaponStatName StatName = Modifier.WeaponItemStatName;
			if (Modifier.WeaponItemType == TargetWeaponItem->GetWeaponItemType())
			{
				float ClampMin = 0.0f;
				float ClampMax = 0.0f;

				switch (StatName)
				{
				case EPepccineWeaponStatName::EPWSN_AttackMultiplier:
					ClampMin = 0.1f;
					ClampMax = 10.0f;
					break;
				case EPepccineWeaponStatName::EPWSN_AttackRange:
					ClampMin = 1000.0f;
					ClampMax = 100000.0f;
					break;
				case EPepccineWeaponStatName::EPWSN_FireRate:
					ClampMin = 0.1f;
					ClampMax = 100.0f;
					break;
				case EPepccineWeaponStatName::EPWSN_ZoomMultiplier:
					ClampMin = 1.0f;
					ClampMax = 10.0f;
					break;
				case EPepccineWeaponStatName::EPWSN_MagazineSize:
					ClampMin = 1.0f;
					ClampMax = 999.0f;
					break;
				case EPepccineWeaponStatName::EPWSN_MagazineAmmo:
					ClampMin = 0.0f;
					ClampMax = 999.0f;
					break;
				case EPepccineWeaponStatName::EPWSN_SpareAmmo:
					ClampMin = 0.0f;
					ClampMax = 999.0f;
					break;
				case EPepccineWeaponStatName::EPWSN_BulletSpeed:
					ClampMin = 10.0f;
					ClampMax = 100000.0f;
					break;
				case EPepccineWeaponStatName::EPWSN_ReloadSpeed:
					ClampMin = 0.1f;
					ClampMax = 10.0f;
					break;
				case EPepccineWeaponStatName::EPWSN_ProjectileCount:
					ClampMin = 1.0f;
					ClampMax = 100.0f;
					break;
				case EPepccineWeaponStatName::EPWSN_BulletSpread:
					ClampMin = 1.0f;
					ClampMax = 10.0f;
					break;
				case EPepccineWeaponStatName::EPWSN_Recoil:
					ClampMin = 1.0f;
					ClampMax = 10.0f;
					break;
				case EPepccineWeaponStatName::EPWSN_Weight:
					ClampMin = 10.0f;
					ClampMax = 1000.0f;
					break;
				}

				GetWeaponItemStatRefByName(TargetWeaponItem, StatName) =
					CalculateTotalValue(StatName, GetWeaponItemStatRefByName(DefaultWeaponItemData, StatName),
					                               ClampMin, ClampMax);
			}
		}
	}
}

float UPepccineWeaponItemManager::CalculateTotalValue(const EPepccineWeaponStatName WeaponItemStatName,
                                                                 float WeaponItemStat, const float ClampMin,
                                                                 const float ClampMax) const
{
	UE_LOG(LogTemp, Warning, TEXT("이전 %s : %.2f"), *UEnum::GetValueAsString(WeaponItemStatName), WeaponItemStat);

	WeaponItemStat += ItemManager->GetTotalSumByWeaponItemStatName(WeaponItemStatName);
	WeaponItemStat *= ItemManager->GetTotalProductByWeaponItemStatName(WeaponItemStatName);

	WeaponItemStat = FMath::Clamp(WeaponItemStat, ClampMin, ClampMax);

	UE_LOG(LogTemp, Warning, TEXT("이후 %s : %.2f"), *UEnum::GetValueAsString(WeaponItemStatName), WeaponItemStat);

	return WeaponItemStat;
}

float& UPepccineWeaponItemManager::GetWeaponItemStatRefByName(UPepccineWeaponItemData* WeaponItemData,
                                                              const EPepccineWeaponStatName WeaponItemStatName)
{
	switch (WeaponItemStatName)
	{
	case EPepccineWeaponStatName::EPWSN_AttackMultiplier:
		return WeaponItemData->GetWeaponItemStatsPointer()->AttackMultiplier;
	case EPepccineWeaponStatName::EPWSN_AttackRange:
		return WeaponItemData->GetWeaponItemStatsPointer()->AttackRange;
	case EPepccineWeaponStatName::EPWSN_FireRate:
		return WeaponItemData->GetWeaponItemStatsPointer()->FireRate;
	case EPepccineWeaponStatName::EPWSN_ZoomMultiplier:
		return WeaponItemData->GetWeaponItemStatsPointer()->ZoomMultiplier;
	case EPepccineWeaponStatName::EPWSN_MagazineSize:
		return WeaponItemData->GetWeaponItemStatsPointer()->MagazineSize;
	case EPepccineWeaponStatName::EPWSN_MagazineAmmo:
		return WeaponItemData->GetWeaponItemStatsPointer()->MagazineAmmo;
	case EPepccineWeaponStatName::EPWSN_SpareAmmo:
		return WeaponItemData->GetWeaponItemStatsPointer()->SpareAmmo;
	case EPepccineWeaponStatName::EPWSN_BulletSpeed:
		return WeaponItemData->GetWeaponItemStatsPointer()->BulletSpeed;
	case EPepccineWeaponStatName::EPWSN_ReloadSpeed:
		return WeaponItemData->GetWeaponItemStatsPointer()->ReloadSpeed;
	case EPepccineWeaponStatName::EPWSN_ProjectileCount:
		return WeaponItemData->GetWeaponItemStatsPointer()->ProjectileCount;
	case EPepccineWeaponStatName::EPWSN_BulletSpread:
		return WeaponItemData->GetWeaponItemStatsPointer()->BulletSpread;
	case EPepccineWeaponStatName::EPWSN_Recoil:
		return WeaponItemData->GetWeaponItemStatsPointer()->Recoil;
	case EPepccineWeaponStatName::EPWSN_Weight:
		return WeaponItemData->GetWeaponItemStatsPointer()->Weight;
	}

	UE_LOG(LogTemp, Error, TEXT("스탯 : %s 이(가) 없습니다."), *UEnum::GetValueAsString(WeaponItemStatName));

	return WeaponItemData->GetWeaponItemStatsPointer()->AttackMultiplier;
}
