// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDoor.h"

#include "PepccineGameInstance.h"
#include "PepccineGameState.h"
#include "Room/Controller/BaseRoomController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Room/RoomManager.h"


ABaseDoor::ABaseDoor()
{
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(RootComponent);
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionObjectType(ECC_WorldDynamic);
	TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ABaseDoor::OnTriggerBeginOverlap);

	PrimaryActorTick.bCanEverTick = false;
}

void ABaseDoor::BeginPlay()
{
	Super::BeginPlay();
	if (APepccineGameState* GameState = Cast<APepccineGameState>(UGameplayStatics::GetGameState(GetWorld())))
	{
		if (ABaseRoomController* RoomController = GameState->GetRoomController())
		{
			RoomController->OnRoomStarted.AddUObject(this, &ABaseDoor::OnStarted);
			RoomController->OnRoomCleared.AddUObject(this, &ABaseDoor::OnCleared);
		}
	}
}

void ABaseDoor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Tags.IsEmpty() && OtherActor->Tags[0] == "Player")
	{
		if (bIsLocked == false)
		{
			if (URoomManager *RoomManager = Cast<UPepccineGameInstance>(GetGameInstance())->GetRoomManager())
			{
				RoomManager->ChangeRoom(GetDirectionRoom());
			}
		}
	}
}

void ABaseDoor::OnStarted()
{
	if (URoomManager *RoomManager = Cast<UPepccineGameInstance>(GetGameInstance())->GetRoomManager())
	{
		const FRoomData* RoomData = GetDirectionRoom();
		if (RoomData->RoomType == ERoomType::ENone)
		{
			Destroy();
			return;
		}
		LockDoor();
	}
}

void ABaseDoor::OnCleared()
{
	
}

FRoomData* ABaseDoor::GetDirectionRoom()
{
	URoomManager *RoomManager = Cast<UPepccineGameInstance>(GetGameInstance())->GetRoomManager();
	if (RoomManager == nullptr) return nullptr;
	FIntPoint RoomPoint = RoomManager->GetCurrentRoomPoint();
	switch (Direction)
	{
	case EDoorDirection::ELeft:
		RoomPoint.X -= 1;
		break;
	case EDoorDirection::ERight:
		RoomPoint.X += 1;
		break;
	case EDoorDirection::EUp:
		RoomPoint.Y -= 1;
		break;
	case EDoorDirection::EDown:
		RoomPoint.Y += 1;
		break;
	}
	return RoomManager->GetPointRoomData(RoomPoint);
}

void ABaseDoor::LockDoor()
{
	// HERE[장훈]: 문이 잠기는 애니메이션 재생 등 추가
	bIsLocked = false;
}

void ABaseDoor::OpenDoor()
{
	// HERE[장훈]: 문이 열리는 애니메이션 재생 등 추가
	bIsLocked = false;
}
