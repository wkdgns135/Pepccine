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

    // ���� ���� ����
    FVector StartLocation = OwnerMonster->GetActorLocation();
    FVector ForwardVector = OwnerMonster->GetActorForwardVector();
    FVector EndLocation = StartLocation + (ForwardVector * AttackRange); // AttackRange �ݿ�

    // ĸ�� ũ�� ����
    float CapsuleRadius = 30.0f;
    float CapsuleHalfHeight = AttackRange * 0.5f; // ���� ������ �°� ����

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

    // ����� ĸ�� �׸��� (�׻� ǥ��)
    DrawDebugCapsule(
        GetWorld(),
        (StartLocation + EndLocation) * 0.5f, // ĸ�� �߽� ��ġ
        CapsuleHalfHeight,
        CapsuleRadius,
        FQuat::Identity,
        bHit ? FColor::Green : FColor::Red, // ��Ʈ ���ο� ���� ���� ����
        false,
        1.0f,
        0,
        1.0f
    );

    // �浹 �� ó��
    if (bHit)
    {
        FVector ImpactPoint = HitResult.ImpactPoint;

        // �浹 ������ ���� ���� �׷��� �ð������� ǥ��
        DrawDebugSphere(
            GetWorld(),
            ImpactPoint,
            CapsuleRadius,
            12,
            FColor::Blue, // �浹 ���� ����
            false,
            1.0f
        );

        // 충돌 대상이 플레이어인 경우 데미지 적용
        if (APepCharacter* Player = Cast<APepCharacter>(HitResult.GetActor()))
        {
            ApplyDamageToTarget(Player, 20.0f);
            SendHitResult(Player, 20.0f, HitResult);
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("No hit detected. Trace from (%s) to (%s)"),
            *StartLocation.ToString(), *EndLocation.ToString());
    }
}
