#include "Monster/Component/MonsterAttackComponent.h"
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

void UMonsterAttackComponent::ApplyDamageToTarget(AActor* Target, float DamageAmount)
{
    if (Target)
    {
        // 데미지 적용
        UGameplayStatics::ApplyDamage(Target, DamageAmount, nullptr, GetOwner(), nullptr);

        APepCharacter* Player = Cast<APepCharacter>(Target);
        if (Player)
        {
            // 플레이어 쪽 피격 반응
        }
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

    // 공격 범위 설정
    FVector StartLocation = OwnerMonster->GetActorLocation();
    FVector ForwardVector = OwnerMonster->GetActorForwardVector();
    FVector EndLocation = StartLocation + (ForwardVector * AttackRange); // AttackRange 반영

    // 캡슐 크기 조정
    float CapsuleRadius = 30.0f;
    float CapsuleHalfHeight = AttackRange * 0.5f; // 공격 범위에 맞게 조정

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(OwnerMonster); // 몬스터는 충돌 무시

    // 캡슐 트레이스 실행
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        FQuat::Identity,
        ECC_Pawn,
        FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
        CollisionParams
    );

    // 디버그 캡슐 그리기 (항상 표시)
    DrawDebugCapsule(
        GetWorld(),
        (StartLocation + EndLocation) * 0.5f, // 캡슐 중심 위치
        CapsuleHalfHeight,
        CapsuleRadius,
        FQuat::Identity,
        bHit ? FColor::Green : FColor::Red, // 히트 여부에 따라 색상 변경
        false,
        1.0f,
        0,
        1.0f
    );

    // 충돌 시 처리
    if (bHit)
    {
        FVector ImpactPoint = HitResult.ImpactPoint;

        // 충돌 지점에 작은 원을 그려서 시각적으로 표시
        DrawDebugSphere(
            GetWorld(),
            ImpactPoint,
            CapsuleRadius,
            12,
            FColor::Blue, // 충돌 지점 색상
            false,
            1.0f
        );

        // 충돌 대상이 플레이어인 경우 데미지 적용
        if (APepCharacter* Player = Cast<APepCharacter>(HitResult.GetActor()))
        {
            ApplyDamageToTarget(Player, 20.0f);
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("No hit detected. Trace from (%s) to (%s)"),
            *StartLocation.ToString(), *EndLocation.ToString());
    }
}
