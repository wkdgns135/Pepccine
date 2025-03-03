#include "Item/PepccineItemManagerComponent.h"

#include "PepccineItemSaveData.h"
#include "PepccineItemSpawner.h"
#include "Active/PepccineActiveItemData.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Passive/PepccinePassiveItemData.h"
#include "Passive/PepccineStatModifier.h"
#include "Resource/PepccineResourceItemData.h"

UPepccineItemManagerComponent::UPepccineItemManagerComponent()
	: ItemSpawner(nullptr), WeaponItemManager(nullptr), PassiveItemManager(nullptr), ActiveItemManager(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPepccineItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// 무기 아이템 매니저 생성
	WeaponItemManager = NewObject<UPepccineWeaponItemManager>();
	// 패시브 아이템 매니저 생성
	PassiveItemManager = NewObject<UPepccinePassiveItemManager>();
	// 액티브 아이템 매니저 생성
	ActiveItemManager = NewObject<UPepccineActiveItemManager>();
	
	if (WeaponItemManager && PassiveItemManager && ActiveItemManager && ItemSpawnerClass)
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

	if (IsActiveItemCooldown())
	{
		const float ActiveItemCooldown = ActiveItemManager->GetActiveItemRemainingCooldown();
		ActiveItemManager->SetActiveItemRemainingCooldown(FMath::Max(ActiveItemCooldown - DeltaTime, 0.0f));
		if (FMath::IsNearlyZero(ActiveItemCooldown))
		{
			ActiveItemManager->SetIsActiveItemCooldown(false);
		}
	}
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
		PassiveItemManager->PickUpItem(PassiveItemData);
	}
	// 액티브 아이템
	else if (const UPepccineActiveItemData* ActiveItemData = Cast<UPepccineActiveItemData>(DropItemData))
	{
		ActiveItemManager->PickUpItem(ActiveItemData);
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

	// 무기 스탯 재설정

	return true;
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

void UPepccineItemManagerComponent::UseActiveItem() const
{
	ActiveItemManager->UseActiveItem();
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
