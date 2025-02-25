// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDoor.h"

#include "PepccineGameState.h"
#include "Room/Controller/BaseRoomController.h"
#include "Kismet/GameplayStatics.h"


ABaseDoor::ABaseDoor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseDoor::BeginPlay()
{
	Super::BeginPlay();
	APepccineGameState* GameState = Cast<APepccineGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		ABaseRoomController* RoomController = GameState->GetRoomController();
		if (RoomController)
		{
			RoomController->OnRoomStarted.AddUObject(this, &ABaseDoor::LockDoor);
			RoomController->OnRoomCleared.AddUObject(this, &ABaseDoor::UnlockDoor);
		}
	}
}

void ABaseDoor::LockDoor()
{
	UE_LOG(LogTemp, Display, TEXT("Lock Door"));
}

void ABaseDoor::UnlockDoor()
{
	UE_LOG(LogTemp, Display, TEXT("Unlock Door"));
}

