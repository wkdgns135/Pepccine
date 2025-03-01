#include "Item/PepccineItemManagerComponent.h"

#include "PepccineItemSaveData.h"
#include "PepccineItemSpawner.h"
#include "Active/PepccineActiveItemData.h"
#include "Item/PepccineItemDataAssetBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Passive/PepccinePassiveItemData.h"
#include "Passive/PepccinePassiveItemDataAsset.h"
#include "Passive/PepccineStatModifier.h"
#include "Resource/PepccineResourceItemData.h"

UPepccineItemManagerComponent::UPepccineItemManagerComponent()
	: ItemSpawner(nullptr), WeaponItemManager(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPepccineItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// 무기 아이템 매니저 생성
	WeaponItemManager = NewObject<UPepccineWeaponItemManager>();
	
	if (WeaponItemManager && ItemSpawnerClass)
	{
		ItemSpawner = NewObject<UPepccineItemSpawner>(GetOwner(), ItemSpawnerClass);
		if (ItemSpawner)
		{
			if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
			{
				// 무기 컴포넌트 등록
				WeaponItemManager->SetWeaponItemComponent(OwnerCharacter);

				// 기본 무기 장착
				WeaponItemManager->EquipDefaultWeapon(ItemSpawner);
			}
		}
	}
}

void UPepccineItemManagerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsActiveItemCooldown)
	{
		ActiveItemRemainingCooldown = FMath::Max(ActiveItemRemainingCooldown - DeltaTime, 0.0f);
		if (FMath::IsNearlyZero(ActiveItemRemainingCooldown))
		{
			bIsActiveItemCooldown = false;
		}
	}
}

float UPepccineItemManagerComponent::GetWeaponStatByName(const EPepccineWeaponItemType WeaponType,
                                                         const EPepccineWeaponStatName WeaponStatName) const
{
	const UPepccineWeaponItemData* WeaponItemData = WeaponItemManager->GetWeaponItemData(WeaponType);

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

bool UPepccineItemManagerComponent::PickUpItem(UPepccineItemDataBase* DropItemData, const bool bIsPlayPickUpSound)
{
	if (!DropItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 데이터가 없습니다."));
		return false;
	}

	// 무기 아이템
	if (const UPepccineWeaponItemData* WeaponItemData = Cast<UPepccineWeaponItemData>(DropItemData))
	{
		WeaponItemManager->PickUpItem(WeaponItemData);
	}
	// 패시브 아이템
	else if (const UPepccinePassiveItemData* PassiveItemData = Cast<UPepccinePassiveItemData>(DropItemData))
	{
		PickUpItem(PassiveItemData);
	}
	// 액티브 아이템
	else if (const UPepccineActiveItemData* ActiveItemData = Cast<UPepccineActiveItemData>(DropItemData))
	{
		PickUpItem(ActiveItemData);
	}
	// 자원 아이템(탄약, 코인)
	else if (const UPepccineResourceItemData* ResourceItemData = Cast<UPepccineResourceItemData>(DropItemData))
	{
		if (ResourceItemData->GetResourceItemType() == EPepccineResourceItemType::EPRIT_AmmoBox)
		{
			WeaponItemManager->GetWeaponItemData(EPepccineWeaponItemType::EPWIT_Main)->GetWeaponItemStatsPointer()->
			                   SpareAmmo += ResourceItemData->GetResourceCount();
		}
		else if (ResourceItemData->GetResourceItemType() == EPepccineResourceItemType::EPRIT_Coin)
		{
			CoinCount += ResourceItemData->GetResourceCount();
		}
	}

	if (bIsPlayPickUpSound)
	{
		if (USoundBase* PickUpSound = DropItemData->GetPickUpSound())
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetOwner()->GetActorLocation());
		}
	}

	// 임시 연사 속도 재설정
	if (UPepccineWeaponItemData* MainWeaponItemData = WeaponItemManager->GetWeaponItemData(
		EPepccineWeaponItemType::EPWIT_Main))
	{
		MainWeaponItemData->GetWeaponItemStatsPointer()->FireRate = GetCalculatedWeaponItemStat(
			EPepccineWeaponItemType::EPWIT_Main, EPepccineWeaponStatName::EPWSN_FireRate);
	}
	if (UPepccineWeaponItemData* SubWeaponItemData = WeaponItemManager->GetWeaponItemData(
		EPepccineWeaponItemType::EPWIT_Sub))
	{
		SubWeaponItemData->GetWeaponItemStatsPointer()->FireRate = GetCalculatedWeaponItemStat(
			EPepccineWeaponItemType::EPWIT_Sub, EPepccineWeaponStatName::EPWSN_FireRate);
	}

	// 무기 스탯 재설정

	return true;
}

void UPepccineItemManagerComponent::PickUpItem(const UPepccinePassiveItemData* PassiveItemData)
{
	// 복사해서 사용
	UPepccinePassiveItemData* NewPassiveItemData = DuplicateObject<UPepccinePassiveItemData>(PassiveItemData, this);

	// 무기 스탯
	IncreaseStatsOperations(NewPassiveItemData->GetWeaponStatModifiers());

	// 캐릭터 스탯
	IncreaseStatsOperations(NewPassiveItemData->GetCharacterStatModifiers());

	UE_LOG(LogTemp, Warning, TEXT("%s 추가"), *NewPassiveItemData->GetDisplayName());

	PassiveItemDatas.Add(NewPassiveItemData->GetItemId(), NewPassiveItemData);
}

void UPepccineItemManagerComponent::PickUpItem(const UPepccineActiveItemData* ActiveItemData)
{
	EquippedActiveItemData = DuplicateObject<UPepccineActiveItemData>(ActiveItemData, this);

	// 획득시 바로 재사용 대기시간 적용
	ActiveItemRemainingCooldown = EquippedActiveItemData->GetCooldown();
	bIsActiveItemCooldown = true;
}

void UPepccineItemManagerComponent::RemovePassiveItemDataById(const int32 ItemId)
{
	const UPepccinePassiveItemData* PassiveItemData = PassiveItemDatas[ItemId];

	DecreaseStatsOperations(PassiveItemData->GetWeaponStatModifiers());
	DecreaseStatsOperations(PassiveItemData->GetCharacterStatModifiers());

	PassiveItemDatas.Remove(ItemId);
}

void UPepccineItemManagerComponent::SwapWeapon(const EPepccineWeaponItemType WeaponType) const
{
	WeaponItemManager->SwapWeapon(WeaponType);
}

void UPepccineItemManagerComponent::FireWeapon(const float WeaponDamage) const
{
	WeaponItemManager->FireWeapon(WeaponDamage);
}

void UPepccineItemManagerComponent::ReloadWeapon() const
{
	WeaponItemManager->ReloadWeapon();
}

float UPepccineItemManagerComponent::GetCalculatedWeaponItemStat(const EPepccineWeaponItemType WeaponItemType,
                                                                 const EPepccineWeaponStatName WeaponItemStatName)
{
	float Result = GetWeaponStatByName(WeaponItemType, WeaponItemStatName);

	Result += GetTotalSumByWeaponItemStatName(WeaponItemStatName);
	Result *= GetTotalProductByWeaponItemStatName(WeaponItemStatName);

	return Result;
}

void UPepccineItemManagerComponent::IncreaseStatsOperations(TArray<FPepccineWeaponStatModifier> Modifiers)
{
	for (const FPepccineWeaponStatModifier& Modifier : Modifiers)
	{
		// 합연산
		if (Modifier.StatModifierDefault.StatModifyType == EPepccineStatModifyType::EPSMT_Add)
		{
			TotalWeaponStatSum.FindOrAdd({Modifier.WeaponItemType, Modifier.WeaponItemStatName}) += Modifier.
				StatModifierDefault.
				StatModifyValue;
		}
		// 곱연산
		else
		{
			TotalWeaponStatProduct.FindOrAdd({Modifier.WeaponItemType, Modifier.WeaponItemStatName}, 1.0f) *= Modifier.
				StatModifierDefault.StatModifyValue;
		}
	}
}

void UPepccineItemManagerComponent::IncreaseStatsOperations(TArray<FPepccineCharacterStatModifier> Modifiers)
{
	for (const FPepccineCharacterStatModifier& Modifier : Modifiers)
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
}

void UPepccineItemManagerComponent::DecreaseStatsOperations(TArray<FPepccineWeaponStatModifier> Modifiers)
{
	for (const FPepccineWeaponStatModifier& Modifier : Modifiers)
	{
		// 합연산
		if (Modifier.StatModifierDefault.StatModifyType == EPepccineStatModifyType::EPSMT_Add)
		{
			if (TotalWeaponStatSum.Find({Modifier.WeaponItemType, Modifier.WeaponItemStatName}))
			{
				TotalWeaponStatSum[{Modifier.WeaponItemType, Modifier.WeaponItemStatName}] -= Modifier.
					StatModifierDefault.
					StatModifyValue;
			}
		}
		// 곱연산
		else
		{
			if (TotalWeaponStatProduct.Find({Modifier.WeaponItemType, Modifier.WeaponItemStatName}))
			{
				TotalWeaponStatProduct[{Modifier.WeaponItemType, Modifier.WeaponItemStatName}] *= Modifier.
					StatModifierDefault.
					StatModifyValue;
			}
		}
	}
}

void UPepccineItemManagerComponent::DecreaseStatsOperations(TArray<FPepccineCharacterStatModifier> Modifiers)
{
	for (const FPepccineCharacterStatModifier& Modifier : Modifiers)
	{
		// 합연산
		if (Modifier.StatModifierDefault.StatModifyType == EPepccineStatModifyType::EPSMT_Add)
		{
			if (TotalCharacterStatSum.Find(Modifier.CharacterStatName))
			{
				TotalCharacterStatSum[Modifier.CharacterStatName] -= Modifier.
				                                                     StatModifierDefault.
				                                                     StatModifyValue;
			}
		}
		// 곱연산
		else
		{
			if (TotalCharacterStatProduct.Find(Modifier.CharacterStatName))
			{
				TotalCharacterStatProduct[Modifier.CharacterStatName] *= Modifier.
				                                                         StatModifierDefault.
				                                                         StatModifyValue;
			}
		}
	}
}

void UPepccineItemManagerComponent::UseActiveItem()
{
	// 장착된 액티브 아이템이 없을 경우
	if (!EquippedActiveItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("액티브 아이템이 없습니다."));
		return;
	}

	// 현재 액티브 아이템 재사용 대기 중일 경우
	if (bIsActiveItemCooldown)
	{
		UE_LOG(LogTemp, Warning, TEXT("액티브 아이템 재사용 대기 중"));
		return;
	}

	// 이미 해당 버프를 받고 있다면 시간만 갱신
	if (GetAppliedPotionItemDataById(EquippedActiveItemData->GetItemId()))
	{
		ActiveItemRemainingCooldown = EquippedActiveItemData->GetCooldown();
		bIsActiveItemCooldown = true;
		return;
	}

	// 포션(버프)
	if (const UPepccinePotionItemData* PotionItemData = Cast<UPepccinePotionItemData>(EquippedActiveItemData))
	{
		ActivatePotionItem(PotionItemData);

		// 지속시간 이후 버프 해제
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("DeactivatePotionItem"), PotionItemData);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, PotionItemData->GetDuration(), false);
	}
}

void UPepccineItemManagerComponent::ActivatePotionItem(const UPepccinePotionItemData* PotionItemData)
{
	IncreaseStatsOperations(PotionItemData->GetWeaponStatModifiers());
	IncreaseStatsOperations(PotionItemData->GetCharacterStatModifiers());
}

void UPepccineItemManagerComponent::DeactivatePotionItem(const UPepccinePotionItemData* PotionItemData)
{
	DecreaseStatsOperations(PotionItemData->GetWeaponStatModifiers());
	DecreaseStatsOperations(PotionItemData->GetCharacterStatModifiers());
}

UPepccinePotionItemData* UPepccineItemManagerComponent::GetAppliedPotionItemDataById(const int32 Id) const
{
	for (UPepccinePotionItemData* PotionItemData : AppliedBuffPotionItemDatas)
	{
		if (PotionItemData->GetItemId() == Id)
		{
			return PotionItemData;
		}
	}

	return nullptr;
}

bool UPepccineItemManagerComponent::UseCoin(const int32 Count)
{
	if (Count > CoinCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("코인이 부족합니다."));
		return false;
	}

	CoinCount -= Count;

	return true;
}


// FPepccineItemSaveData UPepccineItemManagerComponent::GetSaveItemData() const
// {
// 	TArray<int32> PassiveItemIds;
// 	for (auto PassiveItem : PassiveItemDatas)
// 	{
// 		PassiveItemIds.Add(PassiveItem.Key);
// 	}
//
// 	int32 MainWeaponItemId = -1;
// 	FPepccineSaveWeaponAmmo MainWeaponAmmo;
// 	if (MainWeaponItemData)
// 	{
// 		MainWeaponItemId = MainWeaponItemData->GetItemId();
// 		MainWeaponAmmo.MagazinesAmmo = MainWeaponItemData->GetWeaponItemStats().MagazineAmmo;
// 		MainWeaponAmmo.SpareAmmo = MainWeaponItemData->GetWeaponItemStats().SpareAmmo;
// 	}
// 	int32 SubWeaponItemId = -1;
// 	FPepccineSaveWeaponAmmo SubWeaponAmmo;
// 	if (SubWeaponItemData)
// 	{
// 		SubWeaponItemId = SubWeaponItemData->GetItemId();
// 		SubWeaponAmmo.MagazinesAmmo = SubWeaponItemData->GetWeaponItemStats().MagazineAmmo;
// 		SubWeaponAmmo.SpareAmmo = SubWeaponItemData->GetWeaponItemStats().SpareAmmo;
// 	}
// 	const EPepccineWeaponItemType EquippedWeaponItemType = GetEquippedWeaponItemData()
// 		                                                       ? GetEquippedWeaponItemData()->GetWeaponItemType()
// 		                                                       : EPepccineWeaponItemType::EPWIT_Sub;
//
// 	return FPepccineItemSaveData(MainWeaponItemId,
// 	                             MainWeaponAmmo,
// 	                             SubWeaponItemId,
// 	                             SubWeaponAmmo,
// 	                             EquippedWeaponItemType,
// 	                             PassiveItemIds);
// }
//
// void UPepccineItemManagerComponent::LoadItemData(const FPepccineItemSaveData& SaveData)
// {
// 	const UPepccineItemDataAssetBase* ItemDataBase = ItemSpawner->GetItemDataAsset();
// 	if (SaveData.MainWeaponItemId >= 0 && SaveData.MainWeaponItemId < ItemDataBase->GetWeaponItemDataAsset()->
// 		GetWeaponItemDatas().Num())
// 	{
// 		PickUpItem(ItemDataBase->GetWeaponItemDataAsset()->GetWeaponsItemById(SaveData.MainWeaponItemId), false);
// 		MainWeaponItemData->GetWeaponItemStatsPointer()->MagazineAmmo = SaveData.MainWeaponAmmo.MagazinesAmmo;
// 		MainWeaponItemData->GetWeaponItemStatsPointer()->SpareAmmo = SaveData.MainWeaponAmmo.SpareAmmo;
// 	}
// 	else
// 	{
// 		MainWeaponItemData = nullptr;
// 	}
//
// 	if (SaveData.SubWeaponItemId >= 0 && SaveData.SubWeaponItemId < ItemDataBase->GetWeaponItemDataAsset()->
// 		GetWeaponItemDatas().Num())
// 	{
// 		PickUpItem(ItemDataBase->GetWeaponItemDataAsset()->GetWeaponsItemById(SaveData.SubWeaponItemId), false);
// 		SubWeaponItemData->GetWeaponItemStatsPointer()->MagazineAmmo = SaveData.SubWeaponAmmo.MagazinesAmmo;
// 		SubWeaponItemData->GetWeaponItemStatsPointer()->SpareAmmo = SaveData.SubWeaponAmmo.SpareAmmo;
// 	}
//
// 	EquipWeapon(SaveData.EquippedWeaponItemType == EPepccineWeaponItemType::EPWIT_Main
// 		            ? MainWeaponItemData
// 		            : SubWeaponItemData, false);
//
// 	for (const int32 Id : SaveData.PassiveItemIds)
// 	{
// 		const UPepccinePassiveItemData* PassiveItemData = ItemDataBase->GetPassiveItemDataAsset()->
// 		                                                                GetPassiveItemById(Id);
// 		PickUpItem(PassiveItemData);
// 	}
// }
