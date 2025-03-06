#include "Character/Animation/PepccineMontageComponent.h"
#include "Character/Player/PepCharacter.h"

UPepccineMontageComponent::UPepccineMontageComponent()
{
}

void UPepccineMontageComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<APepCharacter>(GetOwner());
	TObjectPtr<class USkeletalMeshComponent> Mesh = Cast<USkeletalMeshComponent>(Owner->GetMesh());
	AnimInstance = Mesh->GetAnimInstance();

	AnimInstance->OnMontageBlendingOut.AddDynamic(this, &UPepccineMontageComponent::OnMontageBlendingOut);
}

void UPepccineMontageComponent::Fire()
{
	if (AnimInstance && FireMontage)
	{
		AnimInstance->Montage_Play(FireMontage);
	}
}

void UPepccineMontageComponent::Reloading(float PlayRate)
{
	if (AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage, PlayRate);
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
	GetWorld()->GetTimerManager().ClearTimer(GetUpTimerHandle);
	
	if (AnimInstance && DeathMontage)
	{
		int32 JumpSection = FMath::RandRange(0, DeathMontage->CompositeSections.Num());
		FName SectionName = FName(*FString::Printf(TEXT("%d"), JumpSection));
		AnimInstance->Montage_Play(DeathMontage);
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}
}

void UPepccineMontageComponent::Draw()
{
	if (AnimInstance && DrawMontage)
	{
		AnimInstance->Montage_Play(DrawMontage);
	}
}

void UPepccineMontageComponent::GetUp()
{
	if (AnimInstance && GetUpMontage)
	{
		AnimInstance->Montage_Play(GetUpMontage);
	}
}

void UPepccineMontageComponent::Pick()
{
	if (AnimInstance && PickMontage)
	{
		AnimInstance->Montage_Play(PickMontage);
	}
}

void UPepccineMontageComponent::Stumble(float time)
{
	if (AnimInstance && StumbleMontage)
	{
		AnimInstance->Montage_Play(StumbleMontage);
		GetWorld()->GetTimerManager().SetTimer(GetUpTimerHandle, this, &UPepccineMontageComponent::StumbleGetUp, time, false);
	}
}

void UPepccineMontageComponent::StumbleGetUp()
{
	if (AnimInstance && StumbleGetUpMontage)
	{
		AnimInstance->Montage_Play(StumbleGetUpMontage);
	}
}

void UPepccineMontageComponent::Climbing()
{
	if (AnimInstance && ClimbingMontage)
	{
		AnimInstance->Montage_Play(ClimbingMontage);
	}
}

void UPepccineMontageComponent::GunHit()
{
	if (AnimInstance && GunHitMontage)
	{
		AnimInstance->Montage_Play(GunHitMontage);
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

void UPepccineMontageComponent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (!Montage || !Owner) return;

	if (Montage == ReloadMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("EndReload!"));
		Owner->bIsReloading = false;
	}
	else if (Montage == DrawMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("EndDraw!"));
		Owner->bIsSwapping = false;
	}
	else if (Montage == GetUpMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("EndGettingUp"));
		Owner->bIsStunning = false;
	}
	else if (Montage == StumbleGetUpMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("EndStumbleGettingUp"));
		Owner->bIsStunning = false;
	}
	else if (Montage == ClimbingMontage)
	{
		UE_LOG(LogTemp, Log, TEXT("EndClimb"));
		Owner->bIsClimbing = false;
	}
}