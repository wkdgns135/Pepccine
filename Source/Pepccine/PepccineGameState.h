// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PepccineGameState.generated.h"

class UBaseRoom;

UCLASS()
class PEPCCINE_API APepccineGameState : public AGameState
{
	GENERATED_BODY()
	
public:

private:
	UPROPERTY()
	TSubclassOf<UBaseRoom> RoomClass;

public:

};
