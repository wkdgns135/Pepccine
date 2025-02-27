#include "Item/PepccineItemManagerComponent.h"

#include "PepccineItemSaveData.h"
#include "PepccineItemSpawner.h"
#include "Item/PepccineItemDataAssetBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Passive/PepccinePassiveItemData.h"
#include "Passive/PepccineStatModifier.h"

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

float UPepccineItemManagerComponent::GetWeaponStatByName(const EPepccineWeaponItemType WeaponType,
                                                         const EPepccineWeaponStatName WeaponStatName) const
{
	UPepccineWeaponItemData* WeaponItemData = WeaponType == EPepccineWeaponItemType::EPWIT_Main
		                                          ? MainWeaponItemData
		                                          : SubWeaponItemData;
	switch (WeaponStatName)
	{
	case EPepccineWeaponStatName::EPWSN_AttackMultiplier: return WeaponItemData->GetWeaponItemStats().AttackMultiplier;
	case EPepccineWeaponStatName::EPWSN_RangeMultiplier: return WeaponItemData->GetWeaponItemStats().RangeMultiplier;
	case EPepccineWeaponStatName::EPWSN_FireRate: return WeaponItemData->GetWeaponItemStats().FireRate;
	case EPepccineWeaponStatName::EPWSN_ZoomMultiplier: return WeaponItemData->GetWeaponItemStats().ZoomMultiplier;
	case EPepccineWeaponStatName::EPWSN_MagazineSize: return WeaponItemData->GetWeaponItemStats().MagazineSize;
	case EPepccineWeaponStatName::EPWSN_MagazineAmmo: return WeaponItemData->GetWeaponItemStats().MagazineAmmo;
	case EPepccineWeaponStatName::EPWSN_SpareAmmo: return WeaponItemData->GetWeaponItemStats().SpareAmmo;
	case EPepccineWeaponStatName::EPWSN_BulletSpeed: return WeaponItemData->GetWeaponItemStats().BulletSpeed;
	case EPepccineWeaponStatName::EPWSN_ReloadSpeed: return WeaponItemData->GetWeaponItemStats().ReloadSpeed;
	case EPepccineWeaponStatName::EPWSN_ProjectileCount: return WeaponItemData->GetWeaponItemStats().ProjectileCount;
	case EPepccineWeaponStatName::EPWSN_BulletSpread: return WeaponItemData->GetWeaponItemStats().BulletSpread;
	case EPepccineWeaponStatName::EPWSN_Recoil: return WeaponItemData->GetWeaponItemStats().Recoil;
	case EPepccineWeaponStatName::EPWSN_Weight: return WeaponItemData->GetWeaponItemStats().Weight;
	}

	return 0.0f;
}

void UPepccineItemManagerComponent::PickUpItem(UPepccineItemDataBase* DropItemData)
{
	if (const UPepccineWeaponItemData* WeaponItemData = Cast<UPepccineWeaponItemData>(DropItemData))
	{
		PickUpItem(WeaponItemData);
	}
	else if (const UPepccinePassiveItemData* PassiveItemData = Cast<UPepccinePassiveItemData>(DropItemData))
	{
		PickUpItem(PassiveItemData);
	}

	// 임시 연사 속도 재설정
	if (MainWeaponItemData)
	{
		MainWeaponItemData->GetWeaponItemStatsPointer()->FireRate = GetCalculatedWeaponItemStat(
			EPepccineWeaponItemType::EPWIT_Main, EPepccineWeaponStatName::EPWSN_FireRate);
	}
	if (SubWeaponItemData)
	{
		SubWeaponItemData->GetWeaponItemStatsPointer()->FireRate = GetCalculatedWeaponItemStat(
			EPepccineWeaponItemType::EPWIT_Sub, EPepccineWeaponStatName::EPWSN_FireRate);
	}

	// 무기 스탯 재설정
}

void UPepccineItemManagerComponent::PickUpItem(const UPepccineWeaponItemData* WeaponItemData)
{
	if (!WeaponItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("무기 아이템 데이터가 없습니다."));
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

	if (USoundBase* PickUpSound = NewWeaponItemData->GetPickUpSound())
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetOwner()->GetActorLocation());
	}

	// 획득한 무기 장착
	EquipWeapon(NewWeaponItemData);

	UE_LOG(LogTemp, Warning, TEXT("%s 등록!"), *NewWeaponItemData->GetDisplayName());
}

void UPepccineItemManagerComponent::PickUpItem(const UPepccinePassiveItemData* PassiveItemData)
{
	// 복사해서 사용
	UPepccinePassiveItemData* NewPassiveItemData = DuplicateObject<UPepccinePassiveItemData>(PassiveItemData, this);

	// 무기 스탯
	for (const FPepccineWeaponStatModifier& Modifier : NewPassiveItemData->GetWeaponStatModifiers())
	{
		// 합연산
		if (Modifier.StatModifierDefault.StatModifyType == EPepccineStatModifyType::EPSMT_Add)
		{
			TotalWeaponStatSum.FindOrAdd({Modifier.WeaponItemType, Modifier.WeaponItemStatName}) += Modifier.
				StatModifierDefault.StatModifyValue;
		}
		// 곱연산
		else
		{
			TotalWeaponStatProduct.FindOrAdd({Modifier.WeaponItemType, Modifier.WeaponItemStatName}, 1.0f) *= Modifier.
				StatModifierDefault.StatModifyValue;
		}
	}

	// 캐릭터 스탯
	for (const FPepccineCharacterStatModifier& Modifier : NewPassiveItemData->GetCharacterStatModifiers())
	{
		// 합연산
		if (Modifier.StatModifierDefault.StatModifyType == EPepccineStatModifyType::EPSMT_Add)
		{
			TotalCharacterStatSum.FindOrAdd(Modifier.CharacterStatName) += Modifier.StatModifierDefault.StatModifyValue;
		}
		// 곱연산
		else
		{
			TotalCharacterStatProduct.FindOrAdd(Modifier.CharacterStatName, 1.0f) *= Modifier.StatModifierDefault.
				StatModifyValue;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%s 추가"), *NewPassiveItemData->GetDisplayName());

	PassiveItemDatas.Add(NewPassiveItemData->GetItemId(), NewPassiveItemData);
}

void UPepccineItemManagerComponent::RemovePassiveItemDataById(const int32 ItemId)
{
	const UPepccinePassiveItemData* PassiveItemData = PassiveItemDatas[ItemId];

	for (const FPepccineWeaponStatModifier& Modifier : PassiveItemData->GetWeaponStatModifiers())
	{
		// 합연산
		if (Modifier.StatModifierDefault.StatModifyType == EPepccineStatModifyType::EPSMT_Add)
		{
			TotalWeaponStatSum[{Modifier.WeaponItemType, Modifier.WeaponItemStatName}] -= Modifier.StatModifierDefault.
				StatModifyValue;
		}
		// 곱연산
		else
		{
			TotalWeaponStatProduct[{Modifier.WeaponItemType, Modifier.WeaponItemStatName}] /= Modifier.
				StatModifierDefault.StatModifyValue;
		}
	}

	PassiveItemDatas.Remove(ItemId);
}

void UPepccineItemManagerComponent::EquipDefaultWeapon()
{
	if (ItemSpawner)
	{
		if (UPepccineItemDataAssetBase* ItemDataAsset = ItemSpawner->GetItemDataAsset())
		{
			if (ItemDataAsset->GetWeaponsItems().Num() > 0)
			{
				// 기본 무기는 0번 인덱스
				if (const UPepccineWeaponItemData* WeaponItemData = ItemDataAsset->GetWeaponsItems()[0])
				{
					PickUpItem(WeaponItemData);
				}
			}
		}
	}
}

void UPepccineItemManagerComponent::EquipWeapon(UPepccineWeaponItemData* Weapon)
{
	// 무기가 없거나 무기 컴포넌트가 없을 경우 장착 X
	if (!Weapon || !WeaponItemComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("교체할 무기가 없거나 무기 컴포넌트가 없습니다."));
		return;
	}

	// 무기 장착
	WeaponItemComp->EquipWeapon(Weapon);

	// 메시 변경
	ChangeWeaponEquippedMesh();

	UE_LOG(LogTemp, Warning, TEXT("%s 장착!"), *Weapon->GetDisplayName());
}

void UPepccineItemManagerComponent::SwapWeapon(const EPepccineWeaponItemType WeaponType)
{
	if (GetEquippedWeaponItemData()->GetWeaponItemType() != WeaponType)
	{
		EquipWeapon(WeaponType == EPepccineWeaponItemType::EPWIT_Main
			            ? MainWeaponItemData
			            : SubWeaponItemData);
	}
}

void UPepccineItemManagerComponent::ChangeWeaponEquippedMesh() const
{
	if (WeaponItemComp)
	{
		if (USkeletalMesh* Mesh = GetEquippedWeaponItemData()->GetEquippedMesh())
		{
			WeaponItemComp->SetSkeletalMesh(Mesh);
		}
	}
}

void UPepccineItemManagerComponent::FireWeapon(float WeaponDamage) const
{
	// 무기 컴포넌트가 있고 장착된 무기가 있을 경우만 발사
	if (WeaponItemComp && GetEquippedWeaponItemData())
	{
		WeaponItemComp->Fire(WeaponDamage);
	}
}

void UPepccineItemManagerComponent::ReloadWeapon() const
{
	// 무기 컴포넌트가 있고 장착된 무기가 있을 경우만 재장전
	if (WeaponItemComp && GetEquippedWeaponItemData())
	{
		if (WeaponItemComp->Reload())
		{
			UPepccineWeaponItemData* EquippedWeaponItemData = GetEquippedWeaponItemData();

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

float UPepccineItemManagerComponent::GetCalculatedWeaponItemStat(const EPepccineWeaponItemType WeaponItemType,
                                                                 const EPepccineWeaponStatName WeaponItemStatName)
{
	float Result = GetWeaponStatByName(WeaponItemType, WeaponItemStatName);

	Result += GetTotalSumByWeaponItemStatName(WeaponItemStatName);
	Result *= GetTotalProductByWeaponItemStatName(WeaponItemStatName);

	return Result;
}

FPepccineItemSaveData UPepccineItemManagerComponent::GetSaveItemData() const
{
	TArray<int32> PassiveItemIds;
	for (auto PassiveItem : PassiveItemDatas)
	{
		PassiveItemIds.Add(PassiveItem.Key);
	}

	const int32 MainWeaponItemId = MainWeaponItemData ? MainWeaponItemData->GetItemId() : -1;
	const int32 SubWeaponItemId = SubWeaponItemData ? SubWeaponItemData->GetItemId() : -1;
	const EPepccineWeaponItemType EquippedWeaponItemType = GetEquippedWeaponItemData()
		                                                       ? GetEquippedWeaponItemData()->GetWeaponItemType()
		                                                       : EPepccineWeaponItemType::EPWIT_Sub;

	return FPepccineItemSaveData(MainWeaponItemId,
	                             SubWeaponItemId,
	                             EquippedWeaponItemType,
	                             PassiveItemIds);
}

void UPepccineItemManagerComponent::LoadItemData(const FPepccineItemSaveData& SaveData)
{
	UPepccineItemDataAssetBase* ItemDataBase = ItemSpawner->GetItemDataAsset();
	if (SaveData.MainWeaponItemId >= 0 && SaveData.MainWeaponItemId < ItemDataBase->GetWeaponsItems().Num())
	{
		PickUpItem(ItemDataBase->GetWeaponsItemById(SaveData.MainWeaponItemId));
	}
	else
	{
		MainWeaponItemData = nullptr;
	}

	if (SaveData.SubWeaponItemId >= 0 && SaveData.SubWeaponItemId < ItemDataBase->GetWeaponsItems().Num())
	{
		PickUpItem(ItemDataBase->GetWeaponsItemById(SaveData.SubWeaponItemId));
	}

	EquipWeapon(SaveData.EquippedWeaponItemType == EPepccineWeaponItemType::EPWIT_Main
		            ? MainWeaponItemData
		            : SubWeaponItemData);

	for (const int32 Id : SaveData.PassiveItemIds)
	{
		const UPepccinePassiveItemData* PassiveItemData = ItemDataBase->GetPassiveItemById(Id);
		PickUpItem(PassiveItemData);
	}
}

void UPepccineItemManagerComponent::SetWeaponItemComponent()
{
	TArray<AActor*> AttachedActors;
	GetOwner()->GetAttachedActors(AttachedActors);

	for (const AActor* Actor : AttachedActors)
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
