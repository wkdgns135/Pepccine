// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseRoomController.generated.h"

class ABaseDoor;
class ASpawner;

DECLARE_MULTICAST_DELEGATE(FOnRoomStarted)
DECLARE_MULTICAST_DELEGATE(FOnRoomCleared)

UENUM()
enum class ERoomShape
{
	ENone,
	E4Way,
	E3Way,
	E2WayParallel,
	E2WayPerpendicular,
	E1Way,
};

UCLASS()
class PEPCCINE_API ABaseRoomController : public AActor
{
	GENERATED_BODY()

public:
	FOnRoomStarted OnRoomStarted;
	FOnRoomCleared OnRoomCleared;

private:
	UPROPERTY(EditAnywhere, Category = "Room")
	ERoomShape RoomShape;
	UPROPERTY(EditAnywhere, Category = "Room")
	TArray<ABaseDoor*> Doors;
	UPROPERTY(EditAnywhere, Category = "Room")
	bool bIsRoomClear;
	UPROPERTY(VisibleInstanceOnly, Category = "Room")
	int MonsterCount;


protected:
	virtual void PostInitializeComponents() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void BeginPlay() override;
	
public:
	void ClearRoom();
	void DecreaseMonsterCount();

private:
	void StartRoom();
	void PlacePlayer();
	void CheckAndClearRoom();
	void InitDoor();
	
public:
	FORCEINLINE bool GetIsRoomClear() const { return bIsRoomClear; }
	FORCEINLINE void IncreaseMonsterCount() { MonsterCount++; }
};

