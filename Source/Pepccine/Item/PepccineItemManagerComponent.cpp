#include "Item/PepccineItemManagerComponent.h"
#include "Item/PepccineItemDataAssetBase.h"
#include "GameFramework/Character.h"
#include "Weapon/PepccineWeaponItemType.h"
#include "Weapon/WeaponStatModifier.h"

UPepccineItemManagerComponent::UPepccineItemManagerComponent()
	: WeaponItemComp(nullptr), ItemDataAsset(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPepccineItemManagerComponent::PickUpWeaponItem(UPepccineWeaponItemData* WeaponItemData)
{
	if (WeaponItemData->GetWeaponItemType() == EPepccineWeaponItemType::EPWIT_Main)
	{
		MainWeaponData = WeaponItemData;
	}
	else
	{
		SubWeaponData = WeaponItemData;
	}
}

void UPepccineItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// 무기 컴포넌트 등록
	SetWeaponItemComponent();

	// 기본 무기 장착
	EquipDefaultWeapon();
}

void UPepccineItemManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPepccineItemManagerComponent::EquipDefaultWeapon()
{
	if (ItemDataAsset && ItemDataAsset->WeaponsItems.Num() > 0)
	{
		// 디버그용 임시 메인 무기 장착
		if (UPepccineWeaponItemData* Weapon = DuplicateObject<UPepccineWeaponItemData>(ItemDataAsset->WeaponsItems[1], this))
		{
			PickUpWeaponItem(Weapon);

			UE_LOG(LogTemp, Warning, TEXT("%s 등록!"), *MainWeaponData->DisplayName);
		}

		// 기본 무기는 0번 인덱스
		if (UPepccineWeaponItemData* Weapon = DuplicateObject<UPepccineWeaponItemData>(ItemDataAsset->WeaponsItems[0], this))
		{
			PickUpWeaponItem(Weapon);

			UE_LOG(LogTemp, Warning, TEXT("%s 등록!"), *SubWeaponData->DisplayName);

			EquipWeapon(SubWeaponData);
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

	UE_LOG(LogTemp, Warning, TEXT("%s 장착!"), *Weapon->DisplayName);
}

void UPepccineItemManagerComponent::SwapWeapon(EPepccineWeaponItemType WeaponType)
{
	if (GetEquippedWeaponData()->GetWeaponItemType() != WeaponType)
	{
		EquipWeapon(WeaponType == EPepccineWeaponItemType::EPWIT_Main ? MainWeaponData : SubWeaponData);
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
