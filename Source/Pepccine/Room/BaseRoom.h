// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseRoom.generated.h"

class ASpawner;

DECLARE_MULTICAST_DELEGATE(FOnRoomEnter)
DECLARE_MULTICAST_DELEGATE(FOnRoomExit)
DECLARE_MULTICAST_DELEGATE(FOnStageEnter)
DECLARE_MULTICAST_DELEGATE(FOnStageExit)

UCLASS()
class PEPCCINE_API UBaseRoom : public UObject
{
	GENERATED_BODY()
	
public:
	FOnRoomEnter OnRoomEnter;
	FOnRoomExit OnRoomExit;

protected:

private:
	UPROPERTY(EditAnywhere, Category = "Room")
	TArray<ASpawner*> Spawners;
	UPROPERTY(EditAnywhere, Category = "Room")
	bool bIsCleared;

public:
	FORCEINLINE bool GetIsCleared() const { return bIsCleared; }
};
