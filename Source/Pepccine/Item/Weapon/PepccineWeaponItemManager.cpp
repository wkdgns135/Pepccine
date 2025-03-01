#include "PepccineWeaponItemManager.h"

#include "PepccineWeaponItemData.h"
#include "GameFramework/Character.h"
#include "Item/PepccineItemDataAssetBase.h"
#include "Item/PepccineItemSpawner.h"

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

void UPepccineWeaponItemManager::EquipDefaultWeapon(const UPepccineItemSpawner* ItemSpawner)
{
	if (ItemSpawner)
	{
		if (const UPepccineItemDataAssetBase* ItemDataAsset = ItemSpawner->GetItemDataAsset())
		{
			if (ItemDataAsset->GetWeaponItemDataAsset()->GetWeaponItemDatas().Num() > 0)
			{
				// 기본 무기는 0번 인덱스
				if (const UPepccineWeaponItemData* WeaponItemData = ItemDataAsset->GetWeaponItemDataAsset()->
					GetWeaponItemDatas()[0])
				{
					PickUpItem(WeaponItemData);
				}
			}
		}
	}
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

void UPepccineWeaponItemManager::FireWeapon(const float WeaponDamage) const
{
	// 무기 컴포넌트가 있고 장착된 무기가 있을 경우만 발사
	if (WeaponItemComp && GetEquippedWeaponItemData())
	{
		WeaponItemComp->Fire(WeaponDamage);
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
