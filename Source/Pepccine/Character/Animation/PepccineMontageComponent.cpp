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

void UPepccineMontageComponent::Death()
{
	if (AnimInstance && DeathMontage)
	{
		int32 JumpSection = FMath::RandRange(0, DeathMontage->CompositeSections.Num());
		FName SectionName = FName(*FString::Printf(TEXT("%d"), JumpSection));
		AnimInstance->Montage_Play(DeathMontage);
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}
}

void UPepccineMontageComponent::Roll(FVector Dir, FRotator ActorRotation)
{
	float ForwardDeltaDegree = 0;

	if (!Dir.IsNearlyZero())
	{
		FMatrix RotMatrix = FRotationMatrix(ActorRotation);
		FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		FVector NormalizedVel = Dir.GetSafeNormal2D();

		float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
		// now get the alpha and convert to degree
		ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		float RightCosAngle = FVector::DotProduct(RightVector, NormalizedVel);
		if (RightCosAngle < 0)
		{
			ForwardDeltaDegree *= -1;
		}

		if (ForwardDeltaDegree < 25.0f && ForwardDeltaDegree > -25.0f)
		{
			ForwardDeltaDegree = 0.0f;
		}
		if (ForwardDeltaDegree < 75.0f && ForwardDeltaDegree > 25.0f)
		{
			ForwardDeltaDegree = 45.0f;
		}
		if (ForwardDeltaDegree < 115.0f && ForwardDeltaDegree > 75.0f)
		{
			ForwardDeltaDegree = 90.0f;
		}
		if (ForwardDeltaDegree < 155.0f && ForwardDeltaDegree > 115.0f)
		{
			ForwardDeltaDegree = 135.0f;
		}
		if (ForwardDeltaDegree < -155.0f || ForwardDeltaDegree > 155.0f)
		{
			ForwardDeltaDegree = 180.0f;
		}
		if (ForwardDeltaDegree > -75.0f && ForwardDeltaDegree < -25.0f)
		{
			ForwardDeltaDegree = -45.0f;
		}
		if (ForwardDeltaDegree > -115.0f && ForwardDeltaDegree < -75.0f)
		{
			ForwardDeltaDegree = -90.0f;
		}
		if (ForwardDeltaDegree > -155.0f && ForwardDeltaDegree < -115.0f)
		{
			ForwardDeltaDegree = -135.0f;
		}
	}

	if (AnimInstance && RollMontage)
	{
		AnimInstance->Montage_Play(RollMontage);
		AnimInstance->Montage_JumpToSection(FName(FString::SanitizeFloat(ForwardDeltaDegree)), RollMontage);
	}

	UE_LOG(LogTemp, Log, TEXT("%s"), *Dir.ToString());
	UE_LOG(LogTemp, Log, TEXT("%.0f"), ForwardDeltaDegree);
}