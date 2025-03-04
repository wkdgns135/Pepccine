#include "Item/PepccineItemManagerComponent.h"

#include "PepccineItemDataAssetBase.h"
#include "PepccineItemSaveData.h"
#include "PepccineItemSpawnerSubSystem.h"
#include "Active/PepccineActiveItemData.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Passive/PepccinePassiveItemData.h"
#include "Passive/PepccineStatModifier.h"
#include "Resource/PepccineResourceItemData.h"

UPepccineItemManagerComponent::UPepccineItemManagerComponent()
	: WeaponItemManager(nullptr), PassiveItemManager(nullptr), ActiveItemManager(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPepccineItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// 무기 아이템 매니저 생성
	WeaponItemManager = NewObject<UPepccineWeaponItemManager>(this);
	WeaponItemManager->InitializeManager(this);
	// 패시브 아이템 매니저 생성
	PassiveItemManager = NewObject<UPepccinePassiveItemManager>(this);
	PassiveItemManager->InitializeManager(this);
	// 액티브 아이템 매니저 생성
	ActiveItemManager = NewObject<UPepccineActiveItemManager>(this);
	ActiveItemManager->InitializeManager(this);

	if (WeaponItemManager && PassiveItemManager && ActiveItemManager)
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
		{
			// 무기 컴포넌트 등록
			WeaponItemManager->SetWeaponItemComponent(OwnerCharacter);

			if (const UPepccineWeaponItemData* WeaponItemData = GetWorld()
			                                                    ->GetSubsystem<UPepccineItemSpawnerSubSystem>()
			                                                    ->GetItemDataAsset()
			                                                    ->GetWeaponItemDataAsset()
			                                                    ->GetWeaponItemDatasById(0))
			{
				// 기본 무기 장착
				WeaponItemManager->EquipDefaultWeapon(WeaponItemData);
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
			                   SpareAmmo += ResourceItemData->GetResourceAmount();
		}
		else if (ResourceItemData->GetResourceItemType() == EPepccineResourceItemType::EPRIT_Coin)
		{
			CoinCount += ResourceItemData->GetResourceAmount();
		}
	}

	if (bIsPlayPickUpSound)
	{
		if (USoundBase* PickUpSound = DropItemData->GetPickUpSound())
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetOwner()->GetActorLocation());
		}
	}

	return true;
}

void UPepccineItemManagerComponent::SwapWeapon(const EPepccineWeaponItemType WeaponType) const
{
	WeaponItemManager->SwapWeapon(WeaponType);
}

void UPepccineItemManagerComponent::FireWeapon(const float WeaponDamage) const
{
	WeaponItemManager->FireWeapon(WeaponDamage, GetShootDirection());
}

void UPepccineItemManagerComponent::ReloadWeapon() const
{
	WeaponItemManager->ReloadWeapon();
}

void UPepccineItemManagerComponent::IncreaseStatsOperations(TArray<FPepccineWeaponStatModifier> Modifiers)
{
	for (const FPepccineWeaponStatModifier& Modifier : Modifiers)
	{
		UPepccineWeaponItemData* TargetWeaponItemData = WeaponItemManager->GetWeaponItemData(
			Modifier.WeaponItemType);

		// 합연산
		if (Modifier.StatModifierDefault.StatModifyType == EPepccineStatModifyType::EPSMT_Add)
		{
			TotalWeaponStatSum.FindOrAdd({Modifier.WeaponItemType, Modifier.WeaponItemStatName})
				+= Modifier.StatModifierDefault.StatModifyValue;

			if (TargetWeaponItemData)
			{
				WeaponItemManager->GetWeaponItemStatRefByName(TargetWeaponItemData, Modifier.WeaponItemStatName)
					+= Modifier.StatModifierDefault.StatModifyValue;
			}
		}
		// 곱연산
		else
		{
			TotalWeaponStatProduct.FindOrAdd({Modifier.WeaponItemType, Modifier.WeaponItemStatName}, 1.0f)
				*= Modifier.StatModifierDefault.StatModifyValue;

			if (TargetWeaponItemData)
			{
				WeaponItemManager->GetWeaponItemStatRefByName(TargetWeaponItemData, Modifier.WeaponItemStatName)
					*= Modifier.StatModifierDefault.StatModifyValue;
			}
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
			TotalCharacterStatProduct.FindOrAdd(Modifier.CharacterStatName, 1.0f)
				*= Modifier.StatModifierDefault.StatModifyValue;
		}
	}
}

void UPepccineItemManagerComponent::DecreaseStatsOperations(TArray<FPepccineWeaponStatModifier> Modifiers)
{
	for (const FPepccineWeaponStatModifier& Modifier : Modifiers)
	{
		UPepccineWeaponItemData* TargetWeaponItemData = WeaponItemManager->GetWeaponItemData(
			Modifier.WeaponItemType);

		// 합연산
		if (Modifier.StatModifierDefault.StatModifyType == EPepccineStatModifyType::EPSMT_Add)
		{
			if (TotalWeaponStatSum.Find({Modifier.WeaponItemType, Modifier.WeaponItemStatName}))
			{
				TotalWeaponStatSum[{Modifier.WeaponItemType, Modifier.WeaponItemStatName}] -= Modifier.
					StatModifierDefault.
					StatModifyValue;

				if (TargetWeaponItemData)
				{
					WeaponItemManager->GetWeaponItemStatRefByName(TargetWeaponItemData, Modifier.WeaponItemStatName)
						-= Modifier.StatModifierDefault.StatModifyValue;
				}
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

				if (TargetWeaponItemData)
				{
					WeaponItemManager->GetWeaponItemStatRefByName(TargetWeaponItemData, Modifier.WeaponItemStatName)
						/= Modifier.StatModifierDefault.StatModifyValue;
				}
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
				TotalCharacterStatSum[Modifier.CharacterStatName] -= Modifier.StatModifierDefault.StatModifyValue;
			}
		}
		// 곱연산
		else
		{
			if (TotalCharacterStatProduct.Find(Modifier.CharacterStatName))
			{
				TotalCharacterStatProduct[Modifier.CharacterStatName] /= Modifier.StatModifierDefault.StatModifyValue;
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

FVector UPepccineItemManagerComponent::GetShootDirection() const
{
	// 머즐 이름
	const FName MuzzleName = GetWeaponItemComp()->GetMuzzleName();
	if (!GetWeaponItemComp()->DoesSocketExist(MuzzleName))
	{
		UE_LOG(LogTemp, Warning, TEXT("일치하는 소캣 \"%s\" 이(가) 없습니다."), *MuzzleName.ToString());
		return FVector::ZeroVector;
	}

	// 머즐 위치
	const FVector MuzzleLocation = GetWeaponItemComp()->GetMuzzleLocation(MuzzleName);

	// 화면 중심 좌표 계산
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ScreenCenter(ViewportSize.X / 2, ViewportSize.Y / 2);

	// 화면 중심에서 월드 방향 가져오기
	FVector WorldLocation, WorldDirection;
	UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetWorld(), 0), ScreenCenter,
	                                         WorldLocation, WorldDirection);

	// Ray 설정
	const FVector Start = WorldLocation;
	const FVector End = Start + (WorldDirection * 10000);
	FHitResult HitResult;

	// DrawDebugLine(GetWorld(), Start, End, FColor::Green, true, 2.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	FVector HitLocation;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldDynamic, QueryParams))
	{
		// 충돌한 위치
		HitLocation = HitResult.Location;
		UE_LOG(LogTemp, Warning, TEXT("Camera Ray Hit! : %s"), *HitResult.GetActor()->GetName());
	}
	else
	{
		// 충돌하지 않으면 Ray 끝점
		HitLocation = End;
	}

	return (HitLocation - MuzzleLocation).GetSafeNormal();
}


// FPepccineItemSaveData UPepccineItemManagerComponent::GetSaveItemData() const
// {
// 	int32 MainWeaponItemId = -1;
// 	FPepccineSaveWeaponAmmo MainWeaponAmmo;
// 	if (const UPepccineWeaponItemData* MainWeaponItemData = WeaponItemManager->GetWeaponItemData(
// 		EPepccineWeaponItemType::EPWIT_Main))
// 	{
// 		MainWeaponItemId = MainWeaponItemData->GetItemId();
// 		MainWeaponAmmo.MagazinesAmmo = MainWeaponItemData->GetWeaponItemStats().MagazineAmmo;
// 		MainWeaponAmmo.SpareAmmo = MainWeaponItemData->GetWeaponItemStats().SpareAmmo;
// 	}
// 	int32 SubWeaponItemId = -1;
// 	FPepccineSaveWeaponAmmo SubWeaponAmmo;
// 	if (const UPepccineWeaponItemData* SubWeaponItemData = WeaponItemManager->GetWeaponItemData(
// 		EPepccineWeaponItemType::EPWIT_Sub))
// 	{
// 		SubWeaponItemId = SubWeaponItemData->GetItemId();
// 		SubWeaponAmmo.MagazinesAmmo = SubWeaponItemData->GetWeaponItemStats().MagazineAmmo;
// 		SubWeaponAmmo.SpareAmmo = SubWeaponItemData->GetWeaponItemStats().SpareAmmo;
// 	}
// 	const EPepccineWeaponItemType EquippedWeaponItemType = GetEquippedWeaponItemData()
// 		                                                       ? GetEquippedWeaponItemData()->GetWeaponItemType()
// 		                                                       : EPepccineWeaponItemType::EPWIT_Sub;
// 	TArray<int32> PassiveItemIds;
// 	for (auto PassiveItem : PassiveItemManager->GetPassiveItemDatas())
// 	{
// 		PassiveItemIds.Add(PassiveItem.Key);
// 	}
// 	int32 ActiveItemId = -1;
// 	if (const UPepccineActiveItemData* ActiveItemData = ActiveItemManager->GetActiveItemData())
// 	{
// 		ActiveItemId = ActiveItemData->GetItemId();
// 	}
//
// 	return FPepccineItemSaveData(MainWeaponItemId,
// 	                             MainWeaponAmmo,
// 	                             SubWeaponItemId,
// 	                             SubWeaponAmmo,
// 	                             EquippedWeaponItemType,
// 	                             PassiveItemIds, ActiveItemId, CoinCount);
// }

// void UPepccineItemManagerComponent::LoadItemData(const FPepccineItemSaveData& SaveData)
// {
// 	UPepccineItemSpawnerSubSystem* ItemSpawnerSubSystem = GetWorld()->GetSubsystem<UPepccineItemSpawnerSubSystem>();
// 	if (!ItemSpawnerSubSystem)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("아이템 스포너 서브시스템이 없습니다."));
// 		return;
// 	}
// 	
// 	const UPepccineItemDataAssetBase* ItemDataBase = ItemSpawnerSubSystem->GetItemDataAsset();
// 	if (!ItemDataBase)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("아이템 데이터 베이스가 없습니다."));
// 		return;
// 	}
// 	
// 	if (SaveData.MainWeaponItemId >= 0 && SaveData.MainWeaponItemId < ItemDataBase->GetWeaponItemDataAsset()->
// 		GetWeaponItemDatas().Num())
// 	{
// 		PickUpItem(ItemDataBase->GetWeaponItemDataAsset()->GetWeaponItemDatasById(SaveData.MainWeaponItemId), false);
// 		if (UPepccineWeaponItemData* MainWeaponItemData = WeaponItemManager->GetWeaponItemData(EPepccineWeaponItemType::EPWIT_Main))
// 		{
// 			MainWeaponItemData->GetWeaponItemStatsPointer()->MagazineAmmo = SaveData.MainWeaponAmmo.MagazinesAmmo;
// 			MainWeaponItemData->GetWeaponItemStatsPointer()->SpareAmmo = SaveData.MainWeaponAmmo.SpareAmmo;
// 		}
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
