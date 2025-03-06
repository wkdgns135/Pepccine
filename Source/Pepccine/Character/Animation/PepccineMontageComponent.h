// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PepccineMontageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UPepccineMontageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPepccineMontageComponent();

	UFUNCTION()
	void Fire();
	UFUNCTION()
	void Reloading(float PlayRate);
	UFUNCTION()
	void Attack();
	UFUNCTION()
	void Death();
	UFUNCTION()
	void Roll(FVector Dir, FRotator ActorRotation);
	UFUNCTION()
	void Draw();
	UFUNCTION()
	void GetUp();
	UFUNCTION()
	void Pick();
	UFUNCTION()
	void Stumble(float time);
	UFUNCTION()
	void StumbleGetUp();
	UFUNCTION()
	void Climbing();
	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	FTimerHandle GetUpTimerHandle;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class APepCharacter> Owner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	UAnimInstance* AnimInstance;
	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* FireMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* ReloadMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* RollMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DrawMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* GetUpMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* PickMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* StumbleMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* StumbleGetUpMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* ClimbingMontage;
};
