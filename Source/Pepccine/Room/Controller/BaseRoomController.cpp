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
	PlacePlayer();
	GetWorldTimerManager().SetTimerForNextTick([this]()
		{
			OnRoomStarted.Broadcast();
		});
}

void ABaseRoomController::ClearRoom()
{
	OnRoomCleared.Broadcast();
}

void ABaseRoomController::PlacePlayer()
{
	URoomManager* RoomManager = Cast<UPepccineGameInstance>(GetGameInstance())->GetRoomManager();
	TArray<AActor*> BaseDoors;
	UGameplayStatics::GetAllActorsOfClass(this, ABaseDoor::StaticClass(), BaseDoors);
	for (AActor* Actor : BaseDoors)
	{
		ABaseDoor* Door = Cast<ABaseDoor>(Actor);
		if (Door && RoomManager)
		{
			if (Door->GetDirectionRoom() == RoomManager->GetPreviousRoomData())
			{
				ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
				if (PlayerCharacter)
				{
					const FVector DoorLocation = Door->GetActorLocation();
					const FRotator DoorRotation = Door->GetActorRotation();
					PlayerCharacter->SetActorLocation(DoorLocation);
					PlayerCharacter->SetActorRotation(DoorRotation);
				}

			}
		}
	}
}
