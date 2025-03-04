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
	PlacePlayer();
	CheckAndClearRoom();
}

void ABaseRoomController::PlacePlayer()
{
	const URoomManager* RoomManager = Cast<UPepccineGameInstance>(GetGameInstance())->GetRoomManager();
	TArray<AActor*> BaseDoors;
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	
	UGameplayStatics::GetAllActorsOfClass(this, ABaseDoor::StaticClass(), BaseDoors);
	for (AActor* Actor : BaseDoors)
	{
		ABaseDoor* Door = Cast<ABaseDoor>(Actor);
		if (Door && RoomManager)
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
			PlayerCharacter->SetActorLocation({0,0,0});
			PlayerCharacter->SetActorRotation({0,0,0});
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
