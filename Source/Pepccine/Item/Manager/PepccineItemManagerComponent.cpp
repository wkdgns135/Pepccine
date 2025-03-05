#include "PepccineItemManagerComponent.h"

#include "Item/PepccineItemDataAssetBase.h"
#include "Item/PepccineItemSaveData.h"
#include "Item/ItemSpawn/PepccineItemSpawnerSubSystem.h"
#include "Item/Active/PepccineActiveItemData.h"
#include "GameFramework/Character.h"
#include "Item/Active/PepccineActiveItemDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Item/Passive/PepccinePassiveItemData.h"
#include "Item/Passive/PepccinePassiveItemDataAsset.h"
#include "Item/Passive/PepccineStatModifier.h"
#include "Item/Resource/PepccineResourceItemData.h"

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

			// 기본 무기 장착
			// WeaponItemManager->EquipDefaultWeapon(
			// 	GetWorld()->GetSubsystem<UPepccineItemSpawnerSubSystem>()->GetDefaultWeaponItemData());

			// 데이터 로드
			if (LoadItemSaveData())
			{
				UE_LOG(LogTemp, Warning, TEXT("아이템 데이터 로드 성공!"));
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

void UPepccineItemManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// 데이터 세이브
	SaveItemSaveData();
}

bool UPepccineItemManagerComponent::LoadItemSaveData()
{
	const UPepccineItemSaveData* SaveData = Cast<UPepccineItemSaveData>(
		UGameplayStatics::LoadGameFromSlot(TEXT("ItemSaveData"), 0));

	if (!SaveData)
	{
		SaveData = Cast<UPepccineItemSaveData>(
			UGameplayStatics::CreateSaveGameObject(UPepccineItemSaveData::StaticClass()));
	}

	FPepccineItemSaveDataStruct SaveDataStruct = SaveData->ItemSaveData;

	const UPepccineItemSpawnerSubSystem* ItemSpawnerSubSystem = GetWorld()->GetSubsystem<
		UPepccineItemSpawnerSubSystem>();
	if (!ItemSpawnerSubSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 스포너 서브시스템이 없습니다."));
		return false;
	}

	const UPepccineItemDataAssetBase* ItemDataBase = ItemSpawnerSubSystem->GetItemDataAsset();
	if (!ItemDataBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 데이터 베이스가 없습니다."));
		return false;
	}

	// 주 무기
	if (SaveDataStruct.MainWeaponItemId >= 0 && SaveDataStruct.MainWeaponItemId < ItemDataBase->GetWeaponItemDataAsset()
		->GetWeaponItemDatas().Num())
	{
		PickUpItem(ItemDataBase->GetWeaponItemDataAsset()->GetWeaponItemDatasById(SaveDataStruct.MainWeaponItemId),
		           false);
		if (UPepccineWeaponItemData* MainWeaponItemData = WeaponItemManager->GetWeaponItemData(
			EPepccineWeaponItemType::EPWIT_Main))
		{
			MainWeaponItemData->GetWeaponItemStatsPointer()->MagazineAmmo = SaveDataStruct.MainWeaponAmmo.MagazinesAmmo;
			MainWeaponItemData->GetWeaponItemStatsPointer()->SpareAmmo = SaveDataStruct.MainWeaponAmmo.SpareAmmo;
		}
	}

	// 보조 무기
	if (SaveDataStruct.SubWeaponItemId >= 0 && SaveDataStruct.SubWeaponItemId < ItemDataBase->GetWeaponItemDataAsset()->
		GetWeaponItemDatas().Num())
	{
		PickUpItem(ItemDataBase->GetWeaponItemDataAsset()->GetWeaponItemDatasById(SaveDataStruct.SubWeaponItemId),
		           false);
		if (UPepccineWeaponItemData* SubWeaponItemData = WeaponItemManager->GetWeaponItemData(
			EPepccineWeaponItemType::EPWIT_Sub))
		{
			SubWeaponItemData->GetWeaponItemStatsPointer()->MagazineAmmo = SaveDataStruct.SubWeaponAmmo.MagazinesAmmo;
			SubWeaponItemData->GetWeaponItemStatsPointer()->SpareAmmo = SaveDataStruct.SubWeaponAmmo.SpareAmmo;
		}
	}

	// 무기 장착
	if (UPepccineWeaponItemData* EquippedWeaponItemData = WeaponItemManager->GetWeaponItemData(
		SaveDataStruct.EquippedWeaponItemType))
	{
		WeaponItemManager->EquipWeapon(EquippedWeaponItemData, false);
	}

	// 패시브
	for (const int32 Id : SaveDataStruct.PassiveItemIds)
	{
		if (Id >= 0 && Id < ItemDataBase->GetWeaponItemDataAsset()->GetWeaponItemDatas().Num())
		{
			if (UPepccinePassiveItemData* PassiveItemData = ItemDataBase->GetPassiveItemDataAsset()->
			                                                              GetPassiveItemById(Id))
			{
				PickUpItem(PassiveItemData);
			}
		}
	}

	// 액티브
	if (SaveDataStruct.ActiveItemId >= 0 && SaveDataStruct.ActiveItemId < ItemDataBase->GetWeaponItemDataAsset()->
		GetWeaponItemDatas().Num())
	{
		if (UPepccineActiveItemData* ActiveItemData = ItemDataBase->GetActiveItemDataAsset()->
		                                                            GetActiveItemById(SaveDataStruct.ActiveItemId))
		{
			PickUpItem(ActiveItemData);
		}
	}

	// 돈
	CoinCount = SaveDataStruct.CoinCount;

	return true;
}

void UPepccineItemManagerComponent::SaveItemSaveData() const
{
	UPepccineItemSaveData* SaveData = Cast<UPepccineItemSaveData>(
		UGameplayStatics::LoadGameFromSlot(TEXT("ItemSaveData"), 0));

	if (SaveData)
	{
		// 주 무기
		if (const UPepccineWeaponItemData* MainWeaponItemData = GetWeaponItemData(
			EPepccineWeaponItemType::EPWIT_Main))
		{
			SaveData->ItemSaveData.MainWeaponItemId = MainWeaponItemData->GetItemId();
			SaveData->ItemSaveData.MainWeaponAmmo.MagazinesAmmo = MainWeaponItemData->GetWeaponItemStats().MagazineAmmo;
			SaveData->ItemSaveData.MainWeaponAmmo.SpareAmmo = MainWeaponItemData->GetWeaponItemStats().SpareAmmo;
		}
		// 보조 무기
		if (const UPepccineWeaponItemData* SubWeaponItemData = GetWeaponItemData(
			EPepccineWeaponItemType::EPWIT_Sub))
		{
			SaveData->ItemSaveData.SubWeaponItemId = SubWeaponItemData->GetItemId();
			SaveData->ItemSaveData.SubWeaponAmmo.MagazinesAmmo = SubWeaponItemData->GetWeaponItemStats().MagazineAmmo;
			SaveData->ItemSaveData.SubWeaponAmmo.SpareAmmo = SubWeaponItemData->GetWeaponItemStats().SpareAmmo;
		}

		// 현재 장착 중인 무기
		if (GetEquippedWeaponItemData())
		{
			SaveData->ItemSaveData.EquippedWeaponItemType = GetEquippedWeaponItemData()->GetWeaponItemType();
		}

		// 패시브
		GetPassiveItemDatas().GenerateKeyArray(SaveData->ItemSaveData.PassiveItemIds);
		// 액티브
		if (const UPepccineActiveItemData* ActiveItemData = GetActiveItemData())
		{
			SaveData->ItemSaveData.ActiveItemId = ActiveItemData->GetItemId();
		}
		
		// 코인
		SaveData->ItemSaveData.CoinCount = CoinCount;

		if (UGameplayStatics::SaveGameToSlot(SaveData, "ItemSaveData", 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("아이템 데이터 저장 성공!"));
		}
	}
}

bool UPepccineItemManagerComponent::PickUpItem(UPepccineItemDataBase* DropItemData, const bool bIsPlayPickUpSound,
                                               const bool bIsShopItem, const int32 Price)
{
	if (!DropItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 데이터가 없습니다."));
		return false;
	}

	// 상점 아이템일 경우
	if (bIsShopItem)
	{
		// 구매 확인 후 구매 못하면 false 리턴
		if (!UseCoin(Price))
		{
			return false;
		}
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

bool UPepccineItemManagerComponent::UseCoin(const int32 Price)
{
	if (Price > CoinCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("코인이 부족 합니다."));
		UE_LOG(LogTemp, Warning, TEXT("필요 : %d / 보유 : %d"), Price, CoinCount);
		return false;
	}

	CoinCount -= Price;

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
	}
	else
	{
		// 충돌하지 않으면 Ray 끝점
		HitLocation = End;
	}

	return (HitLocation - MuzzleLocation).GetSafeNormal();
}
