#include "Item/PepccineItemManagerComponent.h"
#include "Item/PepccineItemDataAssetBase.h"
#include "GameFramework/Character.h"

UPepccineItemManagerComponent::UPepccineItemManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPepccineItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	SetWeaponActor();

	InitializeWeapon();
}

void UPepccineItemManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPepccineItemManagerComponent::InitializeWeapon()
{
	if (ItemDataAsset)
	{
		// 디버그용 임시 메인 무기 장착
		if (UPepccineWeaponItemData* Weapon = DuplicateObject<UPepccineWeaponItemData>(ItemDataAsset->Weapons[1], this))
		{
			MainWeapon = Weapon;

			UE_LOG(LogTemp, Warning, TEXT("%s 등록!"), *MainWeapon->DisplayName);
		}

		// 기본 무기는 0번 인덱스
		if (UPepccineWeaponItemData* Weapon = DuplicateObject<UPepccineWeaponItemData>(ItemDataAsset->Weapons[0], this))
		{
			SubWeapon = Weapon;

			UE_LOG(LogTemp, Warning, TEXT("%s 등록!"), *SubWeapon->DisplayName);

			EquipWeapon(SubWeapon);
		}
	}
}

void UPepccineItemManagerComponent::EquipWeapon(UPepccineWeaponItemData* Weapon)
{
	// 무기가 없을 경우 장착 X
	if (!Weapon)
	{
		return;
	}

	EquippedWeapon = Weapon;

	// 메시 변경
	ChangeWeaponEquippedMesh();

	UE_LOG(LogTemp, Warning, TEXT("%s 장착!"), *EquippedWeapon->DisplayName);
}

void UPepccineItemManagerComponent::SwapWeapon(EPepccineWeaponItemType WeaponType)
{
	if (EquippedWeapon->WeaponItemType != WeaponType)
	{
		EquipWeapon(WeaponType == EPepccineWeaponItemType::EPWIT_Main ? MainWeapon : SubWeapon);
	}
}

void UPepccineItemManagerComponent::ChangeWeaponEquippedMesh()
{
	if (WeaponActor)
	{
		if (USkeletalMeshComponent* WeaponEquippedMesh = WeaponActor->GetComponentByClass<USkeletalMeshComponent>())
		{
			if (USkeletalMesh* Mesh = EquippedWeapon->GetEquippedMesh())
			{
				WeaponEquippedMesh->SetSkeletalMesh(Mesh);
			}
		}
	}
}

void UPepccineItemManagerComponent::FireWeapon()
{
	// 장착중인 무기가 있을 경우만 실행
	if (EquippedWeapon)
	{
		GetEquippedWeapon()->UseItem(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("현재 장착중인 무기가 없습니다."));
	}
}

void UPepccineItemManagerComponent::ReloadWeapon() const
{
	// 장착중인 무기가 있을 경우만 실행
	if (EquippedWeapon)
	{
		GetEquippedWeapon()->Reload();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("현재 장착중인 무기가 없습니다."));
	}
}

void UPepccineItemManagerComponent::SetWeaponActor()
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

