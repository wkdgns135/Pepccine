// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDoor.h"
#include "LockedDoor.generated.h"

UCLASS()
class PEPCCINE_API ALockedDoor : public ABaseDoor
{
	GENERATED_BODY()
	
private:
	virtual void EnterDoor() override;
};
