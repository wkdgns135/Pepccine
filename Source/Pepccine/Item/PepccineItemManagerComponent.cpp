#include "Item/PepccineItemManagerComponent.h"

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

float UPepccineItemManagerComponent::GetWeaponItemStatByName(const FPepccineWeaponStat& WeaponItemStats,
                                                             const EPepccineWeaponStatName StatName)
{
	switch (StatName)
	{
	case EPepccineWeaponStatName::EPWSN_AttackMultiplier: return WeaponItemStats.AttackMultiplier;
	case EPepccineWeaponStatName::EPWSN_RangeMultiplier: return WeaponItemStats.RangeMultiplier;
	case EPepccineWeaponStatName::EPWSN_FireRateMultiplier: return WeaponItemStats.FireRateMultiplier;
	case EPepccineWeaponStatName::EPWSN_ZoomMultiplier: return WeaponItemStats.ZoomMultiplier;
	case EPepccineWeaponStatName::EPWSN_MagazineSize: return WeaponItemStats.MagazineSize;
	case EPepccineWeaponStatName::EPWSN_MagazineAmmo: return WeaponItemStats.MagazineAmmo;
	case EPepccineWeaponStatName::EPWSN_SpareAmmo: return WeaponItemStats.SpareAmmo;
	case EPepccineWeaponStatName::EPWSN_BulletSpeed: return WeaponItemStats.BulletSpeed;
	case EPepccineWeaponStatName::EPWSN_ReloadSpeed: return WeaponItemStats.ReloadSpeed;
	case EPepccineWeaponStatName::EPWSN_ProjectileCount: return WeaponItemStats.ProjectileCount;
	case EPepccineWeaponStatName::EPWSN_BulletSpread: return WeaponItemStats.BulletSpread;
	case EPepccineWeaponStatName::EPWSN_Recoil: return WeaponItemStats.Recoil;
	case EPepccineWeaponStatName::EPWSN_Weight: return WeaponItemStats.Weight;
	}

	UE_LOG(LogTemp, Warning, TEXT("무기 스탯 이름이 존재하지 않습니다."));

	return WeaponItemStats.AttackMultiplier;
}

TObjectPtr<UPepccineWeaponItemData> UPepccineItemManagerComponent::PickUpWeaponItem(
	const UPepccineWeaponItemData* WeaponItemData)
{
	TObjectPtr<UPepccineWeaponItemData> Result = nullptr;

	// 복사해서 사용
	UPepccineWeaponItemData* NewWeaponItemData = DuplicateObject<UPepccineWeaponItemData>(
		WeaponItemData, this);

	if (NewWeaponItemData->GetWeaponItemType() == EPepccineWeaponItemType::EPWIT_Main)
	{
		if (MainWeaponItemData)
		{
			Result = MainWeaponItemData;
		}
		MainWeaponItemData = NewWeaponItemData;
	}
	else
	{
		if (SubWeaponItemData)
		{
			Result = SubWeaponItemData;
		}
		SubWeaponItemData = NewWeaponItemData;
	}

	if (USoundBase* PickUpSound = NewWeaponItemData->GetPickUpSound())
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetOwner()->GetActorLocation());
	}

	// 획득한 무기 장착
	EquipWeapon(NewWeaponItemData);

	UE_LOG(LogTemp, Warning, TEXT("%s 등록!"), *NewWeaponItemData->GetDisplayName());

	return Result;
}

void UPepccineItemManagerComponent::AddPassiveItemData(const UPepccinePassiveItemData* PassiveItemData)
{
	// 복사해서 사용
	UPepccinePassiveItemData* NewPassiveItemData = DuplicateObject<UPepccinePassiveItemData>(PassiveItemData, this);

	NewPassiveItemData->SetPassiveItemId(PassiveItemId);

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

	PassiveItemDatas.Add(PassiveItemId++, NewPassiveItemData);
}

void UPepccineItemManagerComponent::RemovePassiveItemDataById(const int32 Id)
{
	const UPepccinePassiveItemData* PassiveItemData = PassiveItemDatas[Id];

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

	PassiveItemDatas.Remove(Id);
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
					PickUpWeaponItem(WeaponItemData);

					// 디버그용 임시 패시브 적용
					if (const UPepccinePassiveItemData* PassiveItemData = ItemDataAsset->GetPassiveItems()[0])
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
		if (WeaponItemComp->Fire(WeaponDamage))
		{
			UPepccineWeaponItemData* EquippedWeaponData = GetEquippedWeaponItemData();
			EquippedWeaponData->GetWeaponItemStatsPointer()->MagazineAmmo--;

			UE_LOG(LogTemp, Warning, TEXT("%s 발사! %.0f / %.0f"),
			       *EquippedWeaponData->GetDisplayName(),
			       EquippedWeaponData->GetWeaponItemStats().MagazineAmmo,
			       EquippedWeaponData->GetWeaponItemStats().SpareAmmo);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("무기 발사 실패!!"));
		}
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
