#include "PepccineDropItem.h"

#include "PepccineItemDataAssetBase.h"
#include "PepccineItemDataBase.h"
#include "Item/Manager/PepccineItemManagerComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Passive/PepccinePassiveItemData.h"
#include "Resource/PepccineResourceItemData.h"
#include "Weapon/PepccineWeaponItemData.h"

APepccineDropItem::APepccineDropItem()
{
	PrimaryActorTick.bCanEverTick = true;

	// 오버랩 컴포넌트 초기화
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	SphereComp->InitSphereRadius(50.f);
	// init전 콜리전 비활성화
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

	StartLocation = StaticMeshComp->GetRelativeLocation();
}

void APepccineDropItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 스태틱 메쉬 상하 움직임
	CurrentTime += DeltaTime;
	CurrentTime = FMath::Fmod(CurrentTime, 360.0f);

	const float Offset = 20.0f * FMath::Sin(CurrentTime * 0.5f * 2.0f * PI);

	const FVector NewLocation = StartLocation + (GetActorUpVector() * Offset);
	StaticMeshComp->SetRelativeLocation(NewLocation);

	// 스태틱 메쉬 회전
	AddActorLocalRotation(FRotator(0, DeltaTime * 45.0f, 0));
}

void APepccineDropItem::InitializeDropItem(const UPepccineItemDataBase* InDropItemData, const bool bInIsShopItem)
{
	// 데이터 복사 저장
	DropItemData = DuplicateObject<UPepccineItemDataBase>(InDropItemData, this);

	bIsShopItem = bInIsShopItem;
	
	if (DropItemData)
	{
		// 스폰 메시 설정
		StaticMeshComp->SetStaticMesh(DropItemData->GetMeshToSpawn());

		StaticMeshComp->SetRelativeRotation(DropItemData->GetMeshRotationToSpawn());
		StaticMeshComp->SetRelativeScale3D(DropItemData->GetMeshScaleToSpawn());

		// 패시브일 경우 메시 추가 설정
		if (DropItemData->IsA(UPepccinePassiveItemData::StaticClass()))
		{
			StaticMeshComp->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));

			UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(
				StaticMeshComp->GetMaterial(0), this);
			DynMaterial->SetTextureParameterValue(FName("ItemIcon"), DropItemData->GetIconTexture());

			StaticMeshComp->SetMaterial(0, DynMaterial);
		}

		// 콜리전 활성화
		SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void APepccineDropItem::ShowInteractWidget(const bool bShow) const
{
	InteractWidgetComp->SetHiddenInGame(!bShow);
}

void APepccineDropItem::PickUpItem(UPepccineItemManagerComponent* ItemManagerComponent)
{
	if (ItemManagerComponent)
	{
		bool IsDestroy = true;

		if (!DropItemData)
		{
			UE_LOG(LogTemp, Warning, TEXT("아이템 데이터가 없습니다!"));
			return;
		}

		UPepccineWeaponItemData* BeforeWeapon = nullptr;

		// 무기 아이템
		if (const UPepccineWeaponItemData* DropWeapon = Cast<UPepccineWeaponItemData>(DropItemData))
		{
			// 이전 무기 가져오기
			BeforeWeapon = ItemManagerComponent->GetWeaponItemData(DropWeapon->GetWeaponItemType());
		}
		// 자원 아이템
		else if (UPepccineResourceItemData* ResourceItemData = Cast<UPepccineResourceItemData>(DropItemData))
		{
			int32 RemainingCount = 0;

			if (ResourceItemData->GetResourceItemType() == EPepccineResourceItemType::EPRIT_AmmoBox)
			{
				const UPepccineWeaponItemData* MainWeaponData = ItemManagerComponent->GetWeaponItemData(
					EPepccineWeaponItemType::EPWIT_Main);
				if (!MainWeaponData)
				{
					UE_LOG(LogTemp, Warning, TEXT("주 무기가 없습니다."));
					return;
				}
				RemainingCount = (MainWeaponData->GetWeaponItemStats().SpareAmmo + ResourceItemData->
					GetResourceAmount()) - 999;
			}
			else if (ResourceItemData->GetResourceItemType() == EPepccineResourceItemType::EPRIT_Coin)
			{
				RemainingCount = (ItemManagerComponent->GetCoinCount() + ResourceItemData->GetResourceAmount()) - 99;
			}

			if (RemainingCount > 0)
			{
				IsDestroy = false;
				ResourceItemData->SetResourceAmount(ResourceItemData->GetResourceAmount() - RemainingCount);
			}
		}

		// 아이템 획득
		if (!ItemManagerComponent->PickUpItem(DropItemData, true, bIsShopItem))
		{
			return;
		}

		// 이전 무기가 있을 경우
		if (BeforeWeapon)
		{
			IsDestroy = false;
			ChangeWeaponItemData(BeforeWeapon);
		}

		if (IsDestroy)
		{
			// 획득 후 엑터 제거
			Destroy();
		}
	}
}

void APepccineDropItem::ChangeWeaponItemData(UPepccineWeaponItemData* WeaponItemData) const
{
	if (UPepccineWeaponItemData* DropWeaponItemData = Cast<UPepccineWeaponItemData>(DropItemData))
	{
		// 원본 데이터
		const UPepccineWeaponItemData* DefaultWeaponItemData = GetWorld()->
		                                                       GetSubsystem<UPepccineItemSpawnerSubSystem>()->
		                                                       GetItemDataAsset()->GetWeaponItemDataAsset()->
		                                                       GetWeaponItemDatasById(WeaponItemData->GetItemId());
		// 스탯 원본 데이터로 설정
		DropWeaponItemData->SetWeaponStats(DefaultWeaponItemData->GetWeaponItemStats());

		// 현재 탄약 수, 예비 탄약 수는 이전 무기에서 가져오기
		DropWeaponItemData->GetWeaponItemStatsPointer()->MagazineAmmo = WeaponItemData->GetWeaponItemStats().MagazineAmmo;
		DropWeaponItemData->GetWeaponItemStatsPointer()->SpareAmmo = WeaponItemData->GetWeaponItemStats().SpareAmmo;

		// 스태틱 메시 설정
		StaticMeshComp->SetStaticMesh(WeaponItemData->GetMeshToSpawn());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("무기 데이터가 아닙니다."));
	}
}
