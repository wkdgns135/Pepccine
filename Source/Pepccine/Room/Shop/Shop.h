// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shop.generated.h"

UCLASS()
class PEPCCINE_API AShop : public AActor
{
	GENERATED_BODY()

public:
	AShop();

protected:
	virtual void BeginPlay() override;

};
