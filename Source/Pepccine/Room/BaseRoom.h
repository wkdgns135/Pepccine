// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseRoom.generated.h"

class ASpawner;

DECLARE_MULTICAST_DELEGATE(FOnRoomStarted)
DECLARE_MULTICAST_DELEGATE(FOnRoomCleared)

UCLASS()
class PEPCCINE_API ABaseRoom : public AActor
{
	GENERATED_BODY()

public:
	FOnRoomStarted OnRoomStarted;
	FOnRoomCleared OnRoomCleared;

protected:
	virtual void PostInitializeComponents();
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Room")
	TArray<ASpawner*> Spawners;
	UPROPERTY(EditAnywhere, Category = "Room")
	bool bRoomCleared;

public:
	FORCEINLINE bool IsRoomCleared() const { return bRoomCleared; }

};

