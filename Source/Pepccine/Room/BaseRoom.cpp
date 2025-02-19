// Fill out your copyright notice in the Description page of Project Settings.


#include "Room/BaseRoom.h"
#include "PepccineGameState.h"
#include "Kismet/GameplayStatics.h"

void ABaseRoom::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	APepccineGameState* GameState = Cast<APepccineGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		GameState->SetCurrentRoom(this);
	}
}

void ABaseRoom::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimerForNextTick([this]()
		{
			OnRoomStarted.Broadcast();
		});
}
