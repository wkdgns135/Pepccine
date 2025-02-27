// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDoor.h"
#include "FloorDoor.generated.h"

UCLASS()
class PEPCCINE_API AFloorDoor : public ABaseDoor
{
	GENERATED_BODY()

public:
	AFloorDoor();

protected:
	virtual void BeginPlay() override;

private:
	virtual void EnterDoor() override;
	virtual void OnStarted() override;
};
