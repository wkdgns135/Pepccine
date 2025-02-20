// Fill out your copyright notice in the Description page of Project Settings.


#include "Room/BaseGimmick.h"
#include "PepccineGameState.h"
#include "Room/BaseRoom.h"
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
		GameState->GetCurrentRoom()->OnRoomStarted.AddUObject(this, &ABaseGimmick::Activate);
		GameState->GetCurrentRoom()->OnRoomCleared.AddUObject(this, &ABaseGimmick::Deactivate);
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

