#include "PepccineDropItem.h"

#include "PepccineItemDataBase.h"
#include "PepccineItemManagerComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Passive/PepccinePassiveItemData.h"
#include "Weapon/PepccineWeaponItemData.h"

APepccineDropItem::APepccineDropItem()
{
	PrimaryActorTick.bCanEverTick = true;

	// 오버랩 컴포넌트 초기화
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	SphereComp->InitSphereRadius(50.f);

	// 스태틱 메시 컴포넌트 초기화
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(GetRootComponent());
	StaticMeshComp->SetSimulatePhysics(false);

	// 상호작용 위젯 생성
	InteractWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidgetComp"));
	InteractWidgetComp->SetupAttachment(GetRootComponent());
}

void APepccineDropItem::BeginPlay()
{
	Super::BeginPlay();
}

void APepccineDropItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APepccineDropItem::InitializeDropItem(const UPepccineItemDataBase* InDropItemData)
{
	// 데이터 복사 저장
	DropItemData = DuplicateObject<UPepccineItemDataBase>(InDropItemData, this);

	if (DropItemData)
	{
		// 스폰 메시 설정
		StaticMeshComp->SetStaticMesh(DropItemData->GetMeshToSpawn());
		
	}
}

void APepccineDropItem::PickUpItem(UPepccineItemManagerComponent* ItemManagerComponent)
{
	if (ItemManagerComponent)
	{
		bool IsDestroy = true;

		// 무기 아이템
		if (const UPepccineWeaponItemData* WeaponItemData = Cast<UPepccineWeaponItemData>(DropItemData))
		{
			// 교체할 무기가 존재할 경우
			if (UPepccineWeaponItemData* BeforeWeapon = ItemManagerComponent->PickUpWeaponItem(WeaponItemData))
			{
				IsDestroy = false;
				DropItemData = BeforeWeapon;
				StaticMeshComp->SetStaticMesh(BeforeWeapon->GetMeshToSpawn());
			}
		}
		// 패시브 아이템
		else if (const UPepccinePassiveItemData* PassiveItemData = Cast<UPepccinePassiveItemData>(DropItemData))
		{
			ItemManagerComponent->AddPassiveItemData(PassiveItemData);
		}

		if (IsDestroy)
		{
			// 획득 후 엑터 제거
			Destroy();
		}
	}
}
