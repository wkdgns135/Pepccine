// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGimmick.h"
#include "PepccineGameState.h"
#include "Room/Controller/BaseRoomController.h"
#include "Kismet/GameplayStatics.h"

ABaseGimmick::ABaseGimmick()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseGimmick::BeginPlay()
{
	Super::BeginPlay();
	APepccineGameState* GameState = Cast<APepccineGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		ABaseRoomController* RoomController = GameState->GetRoomController();
		if (RoomController)
		{
			RoomController->OnRoomStarted.AddUObject(this, &ABaseGimmick::Activate);
			RoomController->OnRoomCleared.AddUObject(this, &ABaseGimmick::Deactivate);
		}
	}
}

void ABaseGimmick::Activate()
{
	UE_LOG(LogTemp, Display, TEXT("Activate Gimmick"));
}

void ABaseGimmick::Deactivate()
{
	UE_LOG(LogTemp, Display, TEXT("Deactivate Gimmick"));
}

