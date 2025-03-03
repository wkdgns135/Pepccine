// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PepccineGameState.generated.h"

class ABaseRoomController;

UCLASS()
class PEPCCINE_API APepccineGameState : public AGameState
{
	GENERATED_BODY()

public:

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	ABaseRoomController* CurrentRoom;

public:
	FORCEINLINE ABaseRoomController* GetRoomController() const { return CurrentRoom; }
	FORCEINLINE void SetRoomController(ABaseRoomController *Room) { CurrentRoom = Room; }
};
