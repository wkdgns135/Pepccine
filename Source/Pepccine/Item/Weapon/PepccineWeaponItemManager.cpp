#include "PepccineWeaponItemManager.h"

#include "PepccineWeaponItemData.h"
#include "GameFramework/Character.h"
#include "Item/PepccineItemDataAssetBase.h"
#include "Item/PepccineItemManagerComponent.h"
#include "Item/PepccineItemSpawnerSubSystem.h"

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
	PickUpItem(WeaponItemData);
}

void UPepccineWeaponItemManager::PickUpItem(const UPepccineWeaponItemData* WeaponItemData)
{
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
	EquipWeapon(NewWeaponItemData);

	UE_LOG(LogTemp, Warning, TEXT("%s 등록!"), *NewWeaponItemData->GetDisplayName());
}

void UPepccineWeaponItemManager::EquipWeapon(UPepccineWeaponItemData* Weapon, const bool bIsPlayEquipSound) const
{
	// 무기가 없거나 무기 컴포넌트가 없을 경우 장착 X
	if (!Weapon || !WeaponItemComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("교체할 무기가 없거나 무기 컴포넌트가 없습니다."));
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

void UPepccineWeaponItemManager::UpdateWeaponItemStats(EPepccineWeaponItemType WeaponItemType) const
{
	TArray<UPepccinePassiveItemData*> PassiveItemDatas;
	ItemManager->GetPassiveItemDatas().GenerateValueArray(PassiveItemDatas);

	UPepccineWeaponItemData* TargetWeaponItem = WeaponItemType == EPepccineWeaponItemType::EPWIT_Main
		                                            ? MainWeaponItemData
		                                            : SubWeaponItemData;

	UPepccineWeaponItemData* DefaultWeaponItemData = GetWorld()->
	                                                 GetSubsystem<UPepccineItemSpawnerSubSystem>()->
	                                                 GetItemDataAsset()->
	                                                 GetWeaponItemDataAsset()->
	                                                 GetWeaponsItemById(TargetWeaponItem->GetItemId());

	for (const UPepccinePassiveItemData* PassiveItemData : PassiveItemDatas)
	{
		for (const FPepccineWeaponStatModifier Modifier : PassiveItemData->GetWeaponStatModifiers())
		{
			const EPepccineWeaponStatName StatName = Modifier.WeaponItemStatName;
			if (Modifier.WeaponItemType == TargetWeaponItem->GetWeaponItemType())
			{
				GetWeaponItemStatRefByName(TargetWeaponItem, StatName) = CalculateTotalValueFromDefault(
					StatName, GetWeaponItemStatRefByName(DefaultWeaponItemData, StatName));
			}
		}
	}
}

float& UPepccineWeaponItemManager::GetWeaponItemStatRefByName(
	UPepccineWeaponItemData* InWeaponItemData, const EPepccineWeaponStatName WeaponItemStatName)
{
	if (InWeaponItemData)
	{
		switch (WeaponItemStatName)
		{
		case EPepccineWeaponStatName::EPWSN_AttackMultiplier:
			return InWeaponItemData->GetWeaponItemStatsPointer()->AttackMultiplier;
		case EPepccineWeaponStatName::EPWSN_AttackRange:
			return InWeaponItemData->GetWeaponItemStatsPointer()->AttackRange;
		case EPepccineWeaponStatName::EPWSN_FireRate:
			return InWeaponItemData->GetWeaponItemStatsPointer()->FireRate;
		case EPepccineWeaponStatName::EPWSN_ZoomMultiplier:
			return InWeaponItemData->GetWeaponItemStatsPointer()->ZoomMultiplier;
		case EPepccineWeaponStatName::EPWSN_MagazineSize:
			return InWeaponItemData->GetWeaponItemStatsPointer()->MagazineSize;
		case EPepccineWeaponStatName::EPWSN_MagazineAmmo:
			return InWeaponItemData->GetWeaponItemStatsPointer()->MagazineAmmo;
		case EPepccineWeaponStatName::EPWSN_SpareAmmo:
			return InWeaponItemData->GetWeaponItemStatsPointer()->SpareAmmo;
		case EPepccineWeaponStatName::EPWSN_BulletSpeed:
			return InWeaponItemData->GetWeaponItemStatsPointer()->BulletSpeed;
		case EPepccineWeaponStatName::EPWSN_ReloadSpeed:
			return InWeaponItemData->GetWeaponItemStatsPointer()->ReloadSpeed;
		case EPepccineWeaponStatName::EPWSN_ProjectileCount:
			return InWeaponItemData->GetWeaponItemStatsPointer()->ProjectileCount;
		case EPepccineWeaponStatName::EPWSN_BulletSpread:
			return InWeaponItemData->GetWeaponItemStatsPointer()->BulletSpread;
		case EPepccineWeaponStatName::EPWSN_Recoil:
			return InWeaponItemData->GetWeaponItemStatsPointer()->Recoil;
		case EPepccineWeaponStatName::EPWSN_Weight:
			return InWeaponItemData->GetWeaponItemStatsPointer()->Weight;
		}
	}

	return InWeaponItemData->GetWeaponItemStatsPointer()->AttackMultiplier;
}

float UPepccineWeaponItemManager::CalculateTotalValueFromDefault(const EPepccineWeaponStatName WeaponItemStatName,
                                                                 float WeaponItemStat) const
{
	UE_LOG(LogTemp, Warning, TEXT("이전 %s : %.2f"), *UEnum::GetValueAsString(WeaponItemStatName), WeaponItemStat);

	WeaponItemStat += ItemManager->GetTotalSumByWeaponItemStatName(WeaponItemStatName);
	WeaponItemStat *= ItemManager->GetTotalProductByWeaponItemStatName(WeaponItemStatName);

	UE_LOG(LogTemp, Warning, TEXT("이후 %s : %.2f"), *UEnum::GetValueAsString(WeaponItemStatName), WeaponItemStat);

	return WeaponItemStat;
}
