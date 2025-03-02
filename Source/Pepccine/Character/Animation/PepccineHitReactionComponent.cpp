// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/PepccineHitReactionComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UPepccineHitReactionComponent::UPepccineHitReactionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    
    BlendSpeed = 2.0f;
	// ...
}


// Called when the game starts
void UPepccineHitReactionComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACharacter>(GetOwner());
	Mesh = Cast<USkeletalMeshComponent>(Owner->GetMesh());
    Capsule = Cast<UCapsuleComponent>(Owner->GetCapsuleComponent());
    Movement = Owner->GetCharacterMovement();
}


// Called every frame
void UPepccineHitReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsRagdoll)
    {
        UpdateCapsuleDuringRagdoll();
    }
	// ...
}

void UPepccineHitReactionComponent::HitReaction(FName HitBoneName, FVector HitDirection)
{
    if (HitBoneName == "Hips" || !Mesh) return;

    Mesh->SetAllBodiesBelowSimulatePhysics(HitBoneName, true, true);
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    FVector ForceStrength = HitDirection * 5000.0f;
    Mesh->AddForce(ForceStrength, HitBoneName, true);

    CurrentBlendWeight = 0.5f;
    Mesh->SetAllBodiesBelowPhysicsBlendWeight(HitBoneName, CurrentBlendWeight, false, true);

    // 타이머 설정
    GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle, this, &UPepccineHitReactionComponent::UpdatePhysicsBlend, 0.016f, true);
}

void UPepccineHitReactionComponent::UpdatePhysicsBlend()
{
    if (!Mesh) return;

    CurrentBlendWeight = FMath::Max(0.0f, CurrentBlendWeight - BlendSpeed * 0.016f);
    Mesh->SetAllBodiesBelowPhysicsBlendWeight("Hips", CurrentBlendWeight, false, true);

    if (CurrentBlendWeight <= 0.0f)
    {
        GetWorld()->GetTimerManager().ClearTimer(ResetTimerHandle);
        ResetAnimation();
    }
}

void UPepccineHitReactionComponent::ResetAnimation()
{
    if (!Mesh) return;

    Mesh->SetAllBodiesSimulatePhysics(false);
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

//TArray<FHitResult> HitResults;
//FVector Start;
//FVector End;
//FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackRadius);
//
//bool bHit = GetWorld()->SweepMultiByChannel(
//	HitResults,
//	Start,
//	End,
//	FQuat::Identity,
//	ECC_Pawn, // 타격 대상 (Pawn을 대상으로 설정)
//	CollisionShape
//);
// 
//if (bHit)
//{
//	for (const FHitResult& Hit : HitResults)
//	{
//		AActor* HitActor = Hit.GetActor();
//		if (HitActor)
//		{
//			UPrimitiveComponent* HitComponent = Hit.GetComponent();
//			FName HitBoneName = Hit.BoneName; // 본 이름 가져오기
//			FVector HitDirection = (Hit.ImpactPoint - Character->GetActorLocation()).GetSafeNormal();	//충격방향
//			FVector HitDirection = Hit.ImpactNormal;													//둘중에 되는걸로
//		}
//	}
//}

void UPepccineHitReactionComponent::EnterRagdoll(float time)
{
    if (!Mesh) return;

    // 레그돌 활성화
    Mesh->SetSimulatePhysics(true);
    Mesh->SetCollisionProfileName(TEXT("Ragdoll"));

    // 캡슐 콜리전 비활성화 (충돌 방지)
    Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 캐릭터 이동 불가
    Movement->SetMovementMode(EMovementMode::MOVE_None);

    // 캡슐이 레그돌 상태에서 계속 메시를 따라가도록
    bIsRagdoll = true;

    // 일정 시간 후 복구
    GetWorld()->GetTimerManager().SetTimer(RagdollTimerHandle, this, &UPepccineHitReactionComponent::ExitRagdoll, time, false);
}

void UPepccineHitReactionComponent::UpdateCapsuleDuringRagdoll()
{
    if (!Mesh || !Capsule) return;

    // 메시(Ragdoll)에서 가장 아래쪽 본 위치 가져오기 (예: Pelvis)
    FVector MeshLocation = Mesh->GetComponentLocation();

    // 캡슐 콜리전 위치를 본체 아래쪽으로 조정
    FVector AdjustedCapsuleLocation = MeshLocation;
    AdjustedCapsuleLocation.Z += Capsule->GetScaledCapsuleHalfHeight(); // 캡슐 높이 보정

    // 캡슐 위치 업데이트
    Capsule->SetWorldLocation(AdjustedCapsuleLocation);
}

void UPepccineHitReactionComponent::ExitRagdoll()
{
    if (!Mesh || !Capsule) return;

    bIsRagdoll = false;

    // 메시의 현재 위치 가져오기
    FVector MeshLocation = Mesh->GetComponentLocation();
    FRotator MeshRotation = Owner->GetActorRotation();

    // 캡슐을 메시 위치로 이동 및 회전 조정
    FVector AdjustedCapsuleLocation = MeshLocation;
    AdjustedCapsuleLocation.Z += Capsule->GetScaledCapsuleHalfHeight();
    Capsule->SetWorldLocation(AdjustedCapsuleLocation);
    Capsule->SetWorldRotation(FRotator(0.0f, MeshRotation.Yaw, 0.0f));

    // 메시를 캡슐에 다시 부착
    Mesh->AttachToComponent(Capsule, FAttachmentTransformRules::SnapToTargetIncludingScale);
    Mesh->SetSimulatePhysics(false);
    Mesh->SetCollisionProfileName(TEXT("CharacterMesh"));
    Mesh->SetWorldRotation(FRotator(0.0f, MeshRotation.Yaw - 90.0f, 0.0f));

    // 캡슐 콜리전 다시 활성화
    Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    // 이동 가능하도록 설정
    Movement->SetMovementMode(EMovementMode::MOVE_Walking);
}