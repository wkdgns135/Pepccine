#include "ItemManagerComponent.h"
#include "ItemDataAssetBase.h"

UItemManagerComponent::UItemManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	SetWeaponActor();

	InitializeWeapon();
}

void UItemManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UItemManagerComponent::InitializeWeapon()
{
	if (DefaultItemDataAsset)
	{
		// 원본 데이터 에셋 복사
		ItemDataAsset = DuplicateObject<UItemDataAssetBase>(DefaultItemDataAsset, nullptr);

		if (ItemDataAsset)
		{
			// 기본 무기는 0번 인덱스
			if (TObjectPtr<UWeaponItemData> Weapon = ItemDataAsset->Weapons[0])
			{
				SubWeapon = Weapon;
				EquippedWeapon = SubWeapon;

				UE_LOG(LogTemp, Warning, TEXT("보조 무기 %s 등록 및 장착!"), *EquippedWeapon->DisplayName);
			}

			// 디버그용 임시 메인 무기 장착
			if (TObjectPtr<UWeaponItemData> Weapon = ItemDataAsset->Weapons[1])
			{
				MainWeapon = Weapon;

				UE_LOG(LogTemp, Warning, TEXT("주무기 %s 등록!"), *MainWeapon->DisplayName);
			}
		}
	}
}

void UItemManagerComponent::SwapWeapons(EWeaponItemType WeaponItemType)
{
	// 메인 무기가 없으면 스왑 불가능
	// 현재 장착중인 무기 타입과 스왑 하려는 무기 타입과 다를 경우만 실행
	if (MainWeapon && EquippedWeapon->WeaponItemType != WeaponItemType)
	{
		if (WeaponItemType == EWeaponItemType::EWIT_Main)
		{
			EquippedWeapon = MainWeapon;
		}
		else
		{
			EquippedWeapon = SubWeapon;
		}

		UE_LOG(LogTemp, Warning, TEXT("무기 교체! 현재 무기 : %s"), *EquippedWeapon->DisplayName);
	}
}

void UItemManagerComponent::FireWeapon() const
{
	// 장착중인 무기가 있을 경우만 실행
	if (EquippedWeapon)
	{
		GetEquippedWeapon()->UseItem();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("현재 장착중인 무기가 없습니다"));
	}
}

void UItemManagerComponent::ReloadWeapon() const
{
	// 장착중인 무기가 있을 경우만 실행
	if (EquippedWeapon)
	{
		GetEquippedWeapon()->Reload();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("현재 장착중인 무기가 없습니다"));
	}
}

void UItemManagerComponent::SetWeaponActor()
{
	TArray<AActor*> AttachedActors;
	GetOwner()->GetAttachedActors(AttachedActors);

	for (AActor* Actor : AttachedActors)
	{
		if (Actor->IsA(WeaponActorClass))
		{
			UE_LOG(LogTemp, Warning, TEXT("무기 액터 설정 완료!"));
			WeaponActor = Actor;
			break;
		}
	}
}

