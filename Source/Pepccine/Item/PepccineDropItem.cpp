#include "PepccineDropItem.h"

#include "PepccineGameInstance.h"
#include "PepccineItemDataAssetBase.h"
#include "PepccineItemDataBase.h"
#include "PepccineItemPriceWidget.h"
#include "Active/PepccineActiveItemDataAsset.h"
#include "Item/Manager/PepccineItemManagerComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Passive/PepccinePassiveItemData.h"
#include "Resource/PepccineResourceItemData.h"
#include "Room/RoomManager.h"
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

	// 상호작용 위젯 컴포넌트 생성
	InteractWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidgetComp"));
	InteractWidgetComp->SetupAttachment(GetRootComponent());

	// 가격 위젯 컴포넌트 생성
	PriceWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("PriceWidgetComp"));
	PriceWidgetComp->SetupAttachment(GetRootComponent());
}

void APepccineDropItem::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = StaticMeshComp->GetRelativeLocation();

	if (PriceWidgetComp->GetWidget())
	{
		// 가격 위젯 설정
		PriceWidget = Cast<UPepccineItemPriceWidget>(PriceWidgetComp->GetWidget());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("가격 위젯이 설정되지 않았습니다."));
	}
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

void APepccineDropItem::InitializeDropItem(UPepccineItemDataBase* InDropItemData, const bool bInIsShopItem)
{
	// 원본 데이터 저장
	DefaultDropItemData = InDropItemData;
	// 복사본 저장
	DropItemData = DuplicateObject<UPepccineItemDataBase>(InDropItemData, this);

	bIsShopItem = bInIsShopItem;

	if (DropItemData)
	{
		// 상점 아이템일 경우
		if (bIsShopItem)
		{
			// 가격 설정
			ItemPrice = (DropItemData->GetItemRarity() + 1) * 4;

			// 위젯 표시
			PriceWidgetComp->SetHiddenInGame(false);

			if (PriceWidget)
			{
				const FString PriceString = FString::Printf(TEXT("가격: %d"), ItemPrice);
				PriceWidget->PriceText->SetText(FText::FromString(PriceString));
			}
		}

		// 스폰 메시 설정
		StaticMeshComp->SetStaticMesh(DropItemData->GetMeshToSpawn());

		StaticMeshComp->SetRelativeRotation(DropItemData->GetMeshRotationToSpawn());
		StaticMeshComp->SetRelativeScale3D(DropItemData->GetMeshScaleToSpawn());

		// 무기
		if (UPepccineWeaponItemData* WeaponItemData = Cast<UPepccineWeaponItemData>(DropItemData))
		{
			// 탄약 설정
			MagazineAmmo = WeaponItemData->GetWeaponItemStats().MagazineAmmo;
			SpareAmmo = WeaponItemData->GetWeaponItemStats().SpareAmmo;
		}
		// 패시브
		else if (DropItemData->IsA(UPepccinePassiveItemData::StaticClass()))
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

bool APepccineDropItem::PickUpItem(UPepccineItemManagerComponent* ItemManagerComponent)
{
	if (!ItemManagerComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 매니저가 없습니다!"));
		return false;
	}

	if (!DropItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 데이터가 없습니다!"));
		return false;
	}

	bool IsDestroy = true;

	UPepccineWeaponItemData* BeforeWeapon = nullptr;
	UPepccineActiveItemData* BeforeActiveItemData = nullptr;
	// 무기 아이템
	if (const UPepccineWeaponItemData* DropWeapon = Cast<UPepccineWeaponItemData>(DropItemData))
	{
		// 이전 무기 가져오기
		BeforeWeapon = ItemManagerComponent->GetWeaponItemData(DropWeapon->GetWeaponItemType());
	}
	// 액티브 아이템
	else if (const UPepccineActiveItemData* ActiveItemData = Cast<UPepccineActiveItemData>(DropItemData))
	{
		// 이전 액티브 아이템 가져오기
		BeforeActiveItemData = ItemManagerComponent->GetActiveItemData();
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
				return false;
			}
			RemainingCount = (MainWeaponData->GetWeaponItemStats().SpareAmmo + ResourceItemData->
				GetResourceAmount()) - 999;
		}
		else if (ResourceItemData->GetResourceItemType() == EPepccineResourceItemType::EPRIT_Coin)
		{
			RemainingCount = (ItemManagerComponent->GetCoinCount() + ResourceItemData->GetResourceAmount()) - 999;
		}

		if (RemainingCount > 0)
		{
			IsDestroy = false;
			ResourceItemData->SetResourceAmount(ResourceItemData->GetResourceAmount() - RemainingCount);
		}
	}

	// 아이템 획득
	if (!ItemManagerComponent->PickUpItem(DropItemData, true, bIsShopItem, ItemPrice, MagazineAmmo, SpareAmmo))
	{
		return false;
	}

	// 상점 아이템일 경우
	if (bIsShopItem)
	{
		if (const URoomManager* RoomManager = Cast<UPepccineGameInstance>(GetGameInstance())->GetRoomManager())
		{
			TMap<UPepccineItemDataBase*, FVector>& ItemData = RoomManager->GetItemData();
			if (!ItemData.IsEmpty())
			{
				ItemData.Remove(DefaultDropItemData);
			}
		}
	}

	// 이전 무기가 있을 경우
	if (BeforeWeapon)
	{
		IsDestroy = false;
		ChangeWeaponItemData(BeforeWeapon);
	}

	// 이전 액티브 아이템이 있을 경우
	if (BeforeActiveItemData)
	{
		IsDestroy = false;
		ChangeActiveItemData(BeforeActiveItemData);

		if (const UPepccinePotionItemData* PotionItemData = Cast<UPepccinePotionItemData>(BeforeActiveItemData))
		{
			// 버프 적용 중인 경우
			if (ItemManagerComponent->IsAppliedBuff())
			{
				// 버프 제거
				ItemManagerComponent->RemoveBuffEffect(PotionItemData);
			}
		}
	}

	if (IsDestroy)
	{
		// 획득 후 엑터 제거
		Destroy();
	}

	return true;
}

void APepccineDropItem::ChangeWeaponItemData(UPepccineWeaponItemData* BeforeWeaponItemData)
{
	if (DropItemData)
	{
		// 원본 데이터
		DefaultDropItemData = GetWorld()->GetSubsystem<UPepccineItemSpawnerSubSystem>()->
		                                  GetItemDataAsset()->GetWeaponItemDataAsset()->
		                                  GetWeaponItemDatasById(BeforeWeaponItemData->GetItemId());
		// 드랍 아이템 원본 데이터로 설정
		DropItemData = DefaultDropItemData;

		// 현재 탄약 수, 예비 탄약 수는 이전 무기에서 가져오기
		MagazineAmmo = BeforeWeaponItemData->GetWeaponItemStats().MagazineAmmo;
		SpareAmmo = BeforeWeaponItemData->GetWeaponItemStats().SpareAmmo;

		// 스태틱 메시 설정
		StaticMeshComp->SetStaticMesh(BeforeWeaponItemData->GetMeshToSpawn());

		// 상점 아이템일 경우
		if (bIsShopItem)
		{
			PriceWidgetComp->SetHiddenInGame(true);
			bIsShopItem = false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 데이터가 없습니다."));
	}
}

void APepccineDropItem::ChangeActiveItemData(UPepccineActiveItemData* BeforeActiveItemData)
{
	if (DropItemData)
	{
		// 원본 데이터
		DefaultDropItemData = GetWorld()->GetSubsystem<UPepccineItemSpawnerSubSystem>()->
		                                  GetItemDataAsset()->GetActiveItemDataAsset()->
		                                  GetActiveItemById(BeforeActiveItemData->GetItemId());
		// 드랍 아이템 원본 데이터로 설정
		DropItemData = DefaultDropItemData;

		// 스태틱 메시 설정
		StaticMeshComp->SetStaticMesh(BeforeActiveItemData->GetMeshToSpawn());

		// 상점 아이템일 경우
		if (bIsShopItem)
		{
			PriceWidgetComp->SetHiddenInGame(true);
			bIsShopItem = false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 데이터가 없습니다."));
	}
}
