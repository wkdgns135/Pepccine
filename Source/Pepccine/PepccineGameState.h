// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PepccineGameState.generated.h"

class ABaseRoom;

UCLASS()
class PEPCCINE_API APepccineGameState : public AGameState
{
	GENERATED_BODY()

public:

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	ABaseRoom* CurrentRoom;

public:
	FORCEINLINE ABaseRoom* GetCurrentRoom() const { return CurrentRoom; }
	FORCEINLINE void SetCurrentRoom(ABaseRoom *Room) { CurrentRoom = Room; }
};
