// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PepccineGameInstance.generated.h"

class URoomManager;

UCLASS()
class PEPCCINE_API UPepccineGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "RoomManager")
	URoomManager *RoomManager;
	
public:
	FORCEINLINE URoomManager* GetRoomManager() const { return RoomManager; }
};
