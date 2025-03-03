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

	// 스탯 초기 설정
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

	const UPepccineWeaponItemData* DefaultWeaponItemData = GetWorld()->
	                                                       GetSubsystem<UPepccineItemSpawnerSubSystem>()->
	                                                       GetItemDataAsset()->
	                                                       GetWeaponItemDataAsset()->
	                                                       GetWeaponsItemById(TargetWeaponItem->GetItemId());

	for (const UPepccinePassiveItemData* PassiveItemData : PassiveItemDatas)
	{
		for (const FPepccineWeaponStatModifier Modifier : PassiveItemData->GetWeaponStatModifiers())
		{
			switch (const EPepccineWeaponStatName StatName = Modifier.WeaponItemStatName)
			{
			case EPepccineWeaponStatName::EPWSN_AttackMultiplier:
				TargetWeaponItem->GetWeaponItemStatsPointer()->AttackMultiplier = CalculateTotalValueFromDefault(
					StatName, DefaultWeaponItemData->GetWeaponItemStats().AttackMultiplier);
				break;
			case EPepccineWeaponStatName::EPWSN_AttackRange:
				TargetWeaponItem->GetWeaponItemStatsPointer()->AttackRange = CalculateTotalValueFromDefault(
					StatName, DefaultWeaponItemData->GetWeaponItemStats().AttackRange);
				break;
			case EPepccineWeaponStatName::EPWSN_FireRate:
				TargetWeaponItem->GetWeaponItemStatsPointer()->FireRate = CalculateTotalValueFromDefault(
					StatName, DefaultWeaponItemData->GetWeaponItemStats().FireRate);
				break;
			case EPepccineWeaponStatName::EPWSN_ZoomMultiplier:
				TargetWeaponItem->GetWeaponItemStatsPointer()->ZoomMultiplier = CalculateTotalValueFromDefault(
					StatName, DefaultWeaponItemData->GetWeaponItemStats().ZoomMultiplier);
				break;
			case EPepccineWeaponStatName::EPWSN_MagazineSize:
				TargetWeaponItem->GetWeaponItemStatsPointer()->MagazineSize = CalculateTotalValueFromDefault(
					StatName, DefaultWeaponItemData->GetWeaponItemStats().MagazineSize);
				break;
			case EPepccineWeaponStatName::EPWSN_MagazineAmmo:
				TargetWeaponItem->GetWeaponItemStatsPointer()->MagazineAmmo = CalculateTotalValueFromDefault(
					StatName, DefaultWeaponItemData->GetWeaponItemStats().MagazineAmmo);
				break;
			case EPepccineWeaponStatName::EPWSN_SpareAmmo:
				TargetWeaponItem->GetWeaponItemStatsPointer()->SpareAmmo = CalculateTotalValueFromDefault(
					StatName, DefaultWeaponItemData->GetWeaponItemStats().SpareAmmo);
				break;
			case EPepccineWeaponStatName::EPWSN_BulletSpeed:
				TargetWeaponItem->GetWeaponItemStatsPointer()->BulletSpeed = CalculateTotalValueFromDefault(
					StatName, DefaultWeaponItemData->GetWeaponItemStats().BulletSpeed);
				break;
			case EPepccineWeaponStatName::EPWSN_ReloadSpeed:
				TargetWeaponItem->GetWeaponItemStatsPointer()->ReloadSpeed = CalculateTotalValueFromDefault(
					StatName, DefaultWeaponItemData->GetWeaponItemStats().ReloadSpeed);
				break;
			case EPepccineWeaponStatName::EPWSN_ProjectileCount:
				TargetWeaponItem->GetWeaponItemStatsPointer()->ProjectileCount = CalculateTotalValueFromDefault(
					StatName, DefaultWeaponItemData->GetWeaponItemStats().ProjectileCount);
				break;
			case EPepccineWeaponStatName::EPWSN_BulletSpread:
				TargetWeaponItem->GetWeaponItemStatsPointer()->BulletSpread = CalculateTotalValueFromDefault(
					StatName, DefaultWeaponItemData->GetWeaponItemStats().BulletSpread);
				break;
			case EPepccineWeaponStatName::EPWSN_Recoil:
				TargetWeaponItem->GetWeaponItemStatsPointer()->Recoil = CalculateTotalValueFromDefault(
					StatName, DefaultWeaponItemData->GetWeaponItemStats().Recoil);
				break;
			case EPepccineWeaponStatName::EPWSN_Weight:
				TargetWeaponItem->GetWeaponItemStatsPointer()->Weight = CalculateTotalValueFromDefault(
					StatName, DefaultWeaponItemData->GetWeaponItemStats().Weight);
				break;
			}
		}
	}
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
