#include "Monster/Component/MonsterAttackComponent.h"
#include "Monster/Component/MonsterStatComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Player/PepCharacter.h" 
#include "DrawDebugHelpers.h"

UMonsterAttackComponent::UMonsterAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UMonsterAttackComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UMonsterAttackComponent::PerformAttack()
{
    // 소유자를 ACharacter로 변환
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Owner is NOT an ACharacter!"));
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("%s is attacking!"), *OwnerCharacter->GetName());

    if (AttackMontage)
    {
        OwnerCharacter->PlayAnimMontage(AttackMontage);
    }
}

void UMonsterAttackComponent::PlayTransitionMontage()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (AttackTransitionMontage && OwnerCharacter)
    {
        OwnerCharacter->PlayAnimMontage(AttackTransitionMontage);
    }
}

void UMonsterAttackComponent::AttackTrace()
{
    ACharacter* OwnerMonster = Cast<ACharacter>(GetOwner());
    if (OwnerMonster == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Owner is NOT an ACharacter!"));
        return;
    }

    FVector StartLocation = OwnerMonster->GetActorLocation();
    FVector ForwardVector = OwnerMonster->GetActorForwardVector();
    FVector EndLocation = StartLocation + (ForwardVector * AttackRange); // AttackRange �ݿ�

    float CapsuleRadius = 5.0f;
    float CapsuleHalfHeight = 5.0f;

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(OwnerMonster); // 몬스터는 충돌 무시

    // 캡슐 트레이스 실행
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        FQuat::Identity,
        ECC_GameTraceChannel1,
        FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
        CollisionParams
    );

    DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 2.0f, 0, 5.0f);
    /*
        DrawDebugCapsule(
            GetWorld(),
            EndLocation - (ForwardVector * CapsuleRadius * 0.5),
            CapsuleHalfHeight,
            CapsuleRadius,
            FQuat(FRotator(90, 0, 0)),
            bHit ? FColor::Green : FColor::Red,
            false,
            1.0f,
            0,
            1.0f
        );
    */
    if (bHit)
    {
        FVector ImpactPoint = HitResult.ImpactPoint;

        DrawDebugSphere(
            GetWorld(),
            ImpactPoint,
            CapsuleRadius,
            12,
            FColor::Blue,
            false,
            1.0f
        );

        // 충돌 대상이 플레이어인 경우 데미지 적용
        if (APepCharacter* Player = Cast<APepCharacter>(HitResult.GetActor()))
        {
            UMonsterStatComponent* StatComp = OwnerMonster->FindComponentByClass<UMonsterStatComponent>();
            SendHitResult(Player, StatComp->Attack, HitResult);
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("No hit detected. Trace from (%s) to (%s)"),
            *StartLocation.ToString(), *EndLocation.ToString());
    }
}