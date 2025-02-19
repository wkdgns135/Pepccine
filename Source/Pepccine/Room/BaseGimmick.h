// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGimmick.generated.h"

UCLASS()
class PEPCCINE_API ABaseGimmick : public AActor
{
	GENERATED_BODY()

public:	
	ABaseGimmick();

protected:
	virtual void BeginPlay() override;
	virtual void Activate();
	virtual void Deactivate();

};
