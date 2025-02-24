#include "PepccineDropItem.h"

#include "PepccineItemDataBase.h"
#include "Components/SphereComponent.h"

APepccineDropItem::APepccineDropItem()
{
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 컴포넌트 초기화
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->InitSphereRadius(50.f);
	SetRootComponent(SphereComp);

	// 스태틱 메시 컴포넌트 초기화
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(GetRootComponent());
	StaticMeshComp->SetSimulatePhysics(false);
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

void APepccineDropItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void APepccineDropItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

