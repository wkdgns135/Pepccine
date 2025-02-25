// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRoomController.h"
#include "PepccineGameState.h"
#include "Kismet/GameplayStatics.h"

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
	GetWorldTimerManager().SetTimerForNextTick([this]()
		{
			OnRoomStarted.Broadcast();
		});
}

void ABaseRoomController::ClearRoom()
{
	OnRoomCleared.Broadcast();
}
