// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRoomController.h"

#include "PepccineGameInstance.h"
#include "PepccineGameState.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Room/RoomManager.h"
#include "Room/Door/BaseDoor.h"

void ABaseRoomController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	APepccineGameState* GameState = Cast<APepccineGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		GameState->SetRoomController(this);
	}
}

void ABaseRoomController::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr)
		                     ? PropertyChangedEvent.Property->GetFName()
		                     : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ABaseRoomController, RoomShape))
	{
		switch (RoomShape)
		{
		case ERoomShape::E4Way: Doors.SetNum(4);
			break;
		case ERoomShape::E3Way: Doors.SetNum(3);
			break;
		case ERoomShape::E2WayParallel:
		case ERoomShape::E2WayPerpendicular: Doors.SetNum(2);
			break;
		case ERoomShape::E1Way: Doors.SetNum(1);
			break;
		default: Doors.SetNum(0);
			break;
		}
	}
}

void ABaseRoomController::BeginPlay()
{
	Super::BeginPlay();
	MonsterCount = 0;
	GetWorldTimerManager().SetTimerForNextTick(this, &ABaseRoomController::StartRoom);
}

void ABaseRoomController::ClearRoom()
{
	OnRoomCleared.Broadcast();
}

void ABaseRoomController::DecreaseMonsterCount()
{
	MonsterCount--;
	if (MonsterCount <= 0)
	{
		ClearRoom();
	}
}

void ABaseRoomController::StartRoom()
{
	OnRoomStarted.Broadcast();
	InitDoor();
	PlacePlayer();
	CheckAndClearRoom();
}

void ABaseRoomController::PlacePlayer()
{
	const URoomManager* RoomManager = Cast<UPepccineGameInstance>(GetGameInstance())->GetRoomManager();
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (RoomManager && PlayerCharacter)
	{
		for (ABaseDoor* Door : Doors)
		{
			if (Door->GetDirectionRoom() == RoomManager->GetPreviousRoomData())
			{
				if (PlayerCharacter)
				{
					const FVector DoorLocation = Door->GetSpawnPosition();
					const FRotator DoorRotation = Door->GetSpawnRotation();
					PlayerCharacter->SetActorLocation(DoorLocation);
					PlayerCharacter->Controller->SetControlRotation(DoorRotation);
					return;
				}
			}
		}
	}

	// 이전 방 정보가 없을 시 중앙에서 스폰
	if (RoomManager->GetPreviousRoomData() == nullptr)
	{
		if (PlayerCharacter)
		{
			PlayerCharacter->SetActorLocation({0, 0, 50});
			PlayerCharacter->SetActorRotation({0, 0, 0});
		}
	}
}

void ABaseRoomController::CheckAndClearRoom()
{
	if (MonsterCount <= 0)
	{
		ClearRoom();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Monster Count after Broadcast: %d"), MonsterCount);
	}
}

void ABaseRoomController::InitDoor()
{
	UPepccineGameInstance* GameInstance = Cast<UPepccineGameInstance>(GetGameInstance());
	TArray<EDoorDirection> DoorDirections;
	TArray<FIntPoint> Offset = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

	
	if (GameInstance)
	{
		if (URoomManager* RoomManager = GameInstance->GetRoomManager())
		{
			const FIntPoint RoomPoint = RoomManager->GetCurrentRoomPoint();
			for (int i = 0; i < Offset.Num(); i++)
			{
				if (RoomManager->GetCheckRoom(RoomPoint + Offset[i]))
				{
					DoorDirections.Add(static_cast<EDoorDirection>(i));
				}
			}
		}
	}

	for (ABaseDoor *Door : Doors)
	{
		if (Door)
		{
			Door->SetDirection(EDoorDirection::ENone);
		}else return;
	}
	
	// 인접한 문이 4개일 때 (4Way)
	if (DoorDirections.Num() == 4)
	{
		for (int i = 0; i < DoorDirections.Num(); i++)
		{
			Doors[i]->SetDirection(DoorDirections[i]);
		}
	}
	// 인접한 문이 3개일 때 (3Way)
	else if (DoorDirections.Num() == 3)
	{
		if (DoorDirections.Contains(EDoorDirection::EDown) && DoorDirections.Contains(EDoorDirection::EUp))
		{
			Doors[0]->SetDirection(EDoorDirection::EUp);
			Doors[1]->SetDirection(EDoorDirection::EDown);
			if (DoorDirections.Contains(EDoorDirection::ELeft))Doors[2]->SetDirection(EDoorDirection::ELeft);
			if (DoorDirections.Contains(EDoorDirection::ERight))Doors[2]->SetDirection(EDoorDirection::ERight);
		}
		else if (DoorDirections.Contains(EDoorDirection::ELeft) && DoorDirections.Contains(EDoorDirection::ERight))
		{
			Doors[0]->SetDirection(EDoorDirection::ELeft);
			Doors[1]->SetDirection(EDoorDirection::ERight);
			if (DoorDirections.Contains(EDoorDirection::EUp))Doors[2]->SetDirection(EDoorDirection::EUp);
			if (DoorDirections.Contains(EDoorDirection::EDown))Doors[2]->SetDirection(EDoorDirection::EDown);
		}
	}
	// 인접한 문이 2개일 때 (2Way)
	else if (DoorDirections.Num() == 2)
	{
		if (RoomShape == ERoomShape::E4Way || RoomShape == ERoomShape::E3Way)
		{
			if (DoorDirections.Contains(EDoorDirection::EUp) && DoorDirections.Contains(EDoorDirection::EDown))
			{
				Doors[0]->SetDirection(EDoorDirection::EUp);
				Doors[1]->SetDirection(EDoorDirection::EDown);
			}
			else if (DoorDirections.Contains(EDoorDirection::ELeft) && DoorDirections.Contains(EDoorDirection::ERight))
			{
				Doors[0]->SetDirection(EDoorDirection::ELeft);
				Doors[1]->SetDirection(EDoorDirection::ERight);
			}
			else
			{
				Doors[0]->SetDirection(DoorDirections[0]);
				Doors[2]->SetDirection(DoorDirections[1]);
			}
		}
		else
		{
			Doors[0]->SetDirection(DoorDirections[0]);
			Doors[1]->SetDirection(DoorDirections[1]);
		}
	}
	else if (DoorDirections.Num() == 1)
	{
		Doors[0]->SetDirection(DoorDirections[0]);
	}
	
	Doors.RemoveAll([](ABaseDoor* Door) {
		if (Door->GetDirection() == EDoorDirection::ENone)
		{
			Door->Destroy();
			return true;
		}
		return false;
	});	
}
