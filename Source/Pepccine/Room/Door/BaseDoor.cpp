// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDoor.h"

#include "PepccineGameInstance.h"
#include "PepccineGameState.h"
#include "Character/Player/PepCharacter.h"
#include "Room/Controller/BaseRoomController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Room/RoomManager.h"


ABaseDoor::ABaseDoor()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(RootScene);
	
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(RootScene);
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionObjectType(ECC_WorldDynamic);
	TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ABaseDoor::OnTriggerBeginOverlap);
	
	DoorStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorStaticMesh"));
	DoorStaticMesh->SetupAttachment(RootScene);
	SpawnPosition = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPosition"));
	SpawnPosition->SetupAttachment(RootScene);
	
	PrimaryActorTick.bCanEverTick = false;

	bIsLocked = false;
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
	
	SpawnPosition->SetVisibility(false);
}

void ABaseDoor::EnterDoor()
{
	if (URoomManager *RoomManager = Cast<UPepccineGameInstance>(GetGameInstance())->GetRoomManager())
	{
		RoomManager->ChangeRoom(GetDirectionRoom());
	}
}

void ABaseDoor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APepCharacter::StaticClass()))
	{
		if (bIsLocked == false)
		{
			EnterDoor();
		}
	}
}

void ABaseDoor::OnStarted()
{
	bIsLocked = true;
	LockDoor();
	
	if (URoomManager *RoomManager = Cast<UPepccineGameInstance>(GetGameInstance())->GetRoomManager())
	{
		const FRoomData* RoomData = GetDirectionRoom();
		if (RoomData->RoomType == ERoomType::ENone)
		{
			Destroy();
		}
	}
}

void ABaseDoor::OnCleared()
{
	bIsLocked = false;
	OpenDoor();
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

void ABaseDoor::OpenDoor_Implementation(){}
void ABaseDoor::LockDoor_Implementation(){}