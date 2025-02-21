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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class ACharacter> Owner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	UAnimInstance* AnimInstance;
	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* FireMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* ReloadMontage;

	void Fire();
	void Reloading();
};
