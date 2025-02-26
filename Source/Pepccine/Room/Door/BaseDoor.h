// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseDoor.generated.h"

struct FRoomData;
class UBoxComponent;

UENUM()
enum class EDoorDirection : uint8
{
	ELeft,
	ERight,
	EUp,
	EDown
};

UCLASS()
class PEPCCINE_API ABaseDoor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	EDoorDirection Direction;
	// 트리거 볼륨 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* TriggerVolume;

	bool bIsLocked;
	
public:	
	ABaseDoor();
	FRoomData* GetDirectionRoom();
	
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnStarted();
	UFUNCTION()
	void OnCleared();
	
	void LockDoor();
	void OpenDoor();
};
