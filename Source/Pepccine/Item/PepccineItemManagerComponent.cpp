#include "Item/PepccineItemManagerComponent.h"

#include "PepccineItemSpawner.h"
#include "VREditorMode.h"
#include "Item/PepccineItemDataAssetBase.h"
#include "GameFramework/Character.h"
#include "Passive/PepccinePassiveItemData.h"
#include "Weapon/WeaponStatModifier.h"

UPepccineItemManagerComponent::UPepccineItemManagerComponent()
	: ItemSpawner(nullptr), WeaponItemComp(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPepccineItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ItemSpawnerClass)
	{
		ItemSpawner = NewObject<UPepccineItemSpawner>(GetOwner(), ItemSpawnerClass);
		if (ItemSpawner)
		{
			// 무기 컴포넌트 등록
			SetWeaponItemComponent();

			// 기본 무기 장착
			EquipDefaultWeapon();
		}
	}
}

void UPepccineItemManagerComponent::PickUpWeaponItem(const UPepccineWeaponItemData* WeaponItemData)
{
	// 복사해서 사용
	UPepccineWeaponItemData* NewWeaponItemData = DuplicateObject<UPepccineWeaponItemData>(WeaponItemData, this);

	if (NewWeaponItemData->GetWeaponItemType() == EPepccineWeaponItemType::EPWIT_Main)
	{
		MainWeaponItemData = NewWeaponItemData;
	}
	else
	{
		SubWeaponItemData = NewWeaponItemData;
	}

	// 무기 컴포넌트에 있는 스탯 설정
	WeaponItemComp->GetWeaponStatsModifier()->SetWeaponItemStats(NewWeaponItemData);

	UE_LOG(LogTemp, Warning, TEXT("%s 등록!"), *NewWeaponItemData->GetDisplayName());
}

void UPepccineItemManagerComponent::AddPassiveItemData(UPepccinePassiveItemData* PassiveItemData)
{
	PassiveItemData->SetPassiveItemId(PassiveItemId);
	
	PassiveItemDatas.Add(PassiveItemId++, PassiveItemData);
	
	for (const FPepccineWeaponStatModifier& Modifier : PassiveItemData->GetModifyWeaponStats())
	{
		WeaponItemComp->GetWeaponStatsModifier()->ApplyStatModifier(Modifier, true);
	}
}

void UPepccineItemManagerComponent::RemovePassiveItemDataById(const int32 Id)
{
	const UPepccinePassiveItemData* PassiveItemData = PassiveItemDatas[Id];
	PassiveItemDatas.Remove(Id);

	for (FPepccineWeaponStatModifier Modifier : PassiveItemData->GetModifyWeaponStats())
	{
		WeaponItemComp->GetWeaponStatsModifier()->RemoveWeaponStatModifier(Modifier);
	}
}

void UPepccineItemManagerComponent::EquipDefaultWeapon()
{
	if (ItemSpawner)
	{
		if (UPepccineItemDataAssetBase* ItemDataAsset = ItemSpawner->GetItemDataAsset())
		{
			if (ItemDataAsset->GetWeaponsItems().Num() > 0)
			{
				// 디버그용 임시 메인 무기 장착
				if (const UPepccineWeaponItemData* WeaponItemData = ItemDataAsset->GetWeaponsItems()[1])
				{
					PickUpWeaponItem(WeaponItemData);
				}

				// 기본 무기는 0번 인덱스
				if (const UPepccineWeaponItemData* WeaponItemData = ItemDataAsset->GetWeaponsItems()[0])
				{
					PickUpWeaponItem(WeaponItemData);

					EquipWeapon(SubWeaponItemData);

					// 디버그용 임시 패시브 적용
					if (UPepccinePassiveItemData* PassiveItemData = ItemDataAsset->GetPassiveItems()[0])
					{
						AddPassiveItemData(PassiveItemData);
					}
				}
			}
		}
	}
}

void UPepccineItemManagerComponent::EquipWeapon(UPepccineWeaponItemData* Weapon)
{
	// 무기가 없고 무기 컴포넌트가 없을 경우 장착 X
	if (!Weapon && !WeaponItemComp)
	{
		return;
	}

	// 무기 장착
	WeaponItemComp->EquipWeapon(Weapon);

	// 메시 변경
	ChangeWeaponEquippedMesh();

	UE_LOG(LogTemp, Warning, TEXT("%s 장착!"), *Weapon->GetDisplayName());
}

void UPepccineItemManagerComponent::SwapWeapon(EPepccineWeaponItemType WeaponType)
{
	if (GetEquippedWeaponData()->GetWeaponItemType() != WeaponType)
	{
		EquipWeapon(WeaponType == EPepccineWeaponItemType::EPWIT_Main ? MainWeaponItemData : SubWeaponItemData);
	}
}

void UPepccineItemManagerComponent::ChangeWeaponEquippedMesh() const
{
	if (WeaponItemComp)
	{
		if (USkeletalMesh* Mesh = GetEquippedWeaponData()->GetEquippedMesh())
		{
			WeaponItemComp->SetSkeletalMesh(Mesh);
		}
	}
}

void UPepccineItemManagerComponent::FireWeapon() const
{
	// 무기 컴포넌트가 있고 장착된 무기가 있을 경우만 발사
	if (WeaponItemComp && GetEquippedWeaponData())
	{
		WeaponItemComp->Fire();
	}
}

void UPepccineItemManagerComponent::ReloadWeapon() const
{
	// 무기 컴포넌트가 있고 장착된 무기가 있을 경우만 재장전
	if (WeaponItemComp && GetEquippedWeaponData())
	{
		WeaponItemComp->Reload();
	}
}

void UPepccineItemManagerComponent::SetWeaponItemComponent()
{
	TArray<AActor*> AttachedActors;
	GetOwner()->GetAttachedActors(AttachedActors);

	for (AActor* Actor : AttachedActors)
	{
		if (Actor)
		{
			if (UPepccineWeaponItemComponent* Comp = Actor->FindComponentByClass<UPepccineWeaponItemComponent>())
			{
				// 무기 컴포넌트 등록
				WeaponItemComp = Comp;
				// 캐릭터 설정
				WeaponItemComp->SetCharacter(Cast<ACharacter>(GetOwner()));

				UE_LOG(LogTemp, Warning, TEXT("무기 컴포넌트 설정 완료!"));
				break;
			}
		}
	}
}
