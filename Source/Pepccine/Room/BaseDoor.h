// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseDoor.generated.h"

UCLASS()
class PEPCCINE_API ABaseDoor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	int Direction;
	
public:	
	ABaseDoor();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void LockDoor();
	UFUNCTION()
	void UnlockDoor();
};
