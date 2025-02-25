// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/PepccineMontageComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UPepccineMontageComponent::UPepccineMontageComponent()
{
}

// Called when the game starts
void UPepccineMontageComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACharacter>(GetOwner());
	TObjectPtr<class USkeletalMeshComponent> Mesh = Cast<USkeletalMeshComponent>(Owner->GetMesh());
	AnimInstance = Mesh->GetAnimInstance();
}

void UPepccineMontageComponent::Fire()
{
	if (AnimInstance && FireMontage)
	{
		AnimInstance->Montage_Play(FireMontage);
	}
}

void UPepccineMontageComponent::Reloading()
{
	if (AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage);
	}
}

void UPepccineMontageComponent::Attack()
{
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
	}
}

void UPepccineMontageComponent::Roll()
{
	if (AnimInstance && RollMontage)
	{
		AnimInstance->Montage_Play(RollMontage);
	}
}