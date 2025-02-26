// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseRoomController.generated.h"

class ASpawner;

DECLARE_MULTICAST_DELEGATE(FOnRoomStarted)
DECLARE_MULTICAST_DELEGATE(FOnRoomCleared)

UCLASS()
class PEPCCINE_API ABaseRoomController : public AActor
{
	GENERATED_BODY()

public:
	FOnRoomStarted OnRoomStarted;
	FOnRoomCleared OnRoomCleared;

private:
	UPROPERTY(EditAnywhere, Category = "Room")
	bool bIsRoomClear;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
public:
	void ClearRoom();
	
public:
	FORCEINLINE bool GetIsRoomClear() const { return bIsRoomClear; }

};

