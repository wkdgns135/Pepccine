// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PepccineHitReactionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UPepccineHitReactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPepccineHitReactionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<class ACharacter> Owner;
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<class USkeletalMeshComponent> Mesh;
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<class UCapsuleComponent> Capsule;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class UCharacterMovementComponent> Movement;

	float CurrentBlendWeight;
	float BlendSpeed;

	FTimerHandle ResetTimerHandle;
	FTimerHandle RagdollTickTimerHandle;
	FTimerHandle RagdollTimerHandle;
	
	bool bIsRagdoll;

	void UpdatePhysicsBlend();
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void HitReaction(FName HitBoneName, FVector HitDirection);
	UFUNCTION(BlueprintCallable)
	void ResetAnimation();
	UFUNCTION(BlueprintCallable)
	void EnterRagdoll(float time);
	UFUNCTION(BlueprintCallable)
	void UpdateCapsuleDuringRagdoll();
	UFUNCTION(BlueprintCallable)
	void ExitRagdoll();


};
