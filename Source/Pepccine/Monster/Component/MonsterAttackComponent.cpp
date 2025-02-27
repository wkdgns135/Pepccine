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
    if (OwnerCharacter == nullptr)
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

void UMonsterAttackComponent::PlayAttackMontage()
{
    if (AttackMontage && GetOwner())
    {
        AActor* Owner = GetOwner();
        USkeletalMeshComponent* MeshComp = Owner->FindComponentByClass<USkeletalMeshComponent>();

        if (MeshComp && MeshComp->GetAnimInstance())
        {
            UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
            AnimInstance->Montage_Play(AttackMontage);
        }
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
    FVector EndLocation = StartLocation + (ForwardVector * 100.0f);  // 공격 범위 

    // 캡슐 트레이스 매개변수
    float CapsuleRadius = 30.0f; 
    float CapsuleHalfHeight = 100.0f; 

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

    // 충돌 시 처리
    if (bHit)
    {
        FVector ImpactPoint = HitResult.ImpactPoint;

        // 충돌 지점에 작은 원을 그려서 시각적으로 표시 (충돌이 있을 때만)
        DrawDebugSphere(
            GetWorld(),
            ImpactPoint,
            CapsuleRadius,
            12, 
            FColor::Green,  
            false, 
            1.0f  
        );

        // 충돌 대상이 플레이어인 경우 데미지 적용
        if (APepCharacter* Player = Cast<APepCharacter>(HitResult.GetActor()))
        {
            ApplyDamageToTarget(Player, 20.0f);  // 예시: 20의 데미지 적용
        }
    }
    else
    {
        DrawDebugCapsule(
            GetWorld(),
            EndLocation, 
            CapsuleHalfHeight, 
            CapsuleRadius,
            FQuat::Identity,
            FColor::Red,
            false, 
            1.0f,  
            0,     
            1.0f  
        );

        // 로그 출력
        UE_LOG(LogTemp, Log, TEXT("No hit detected. Trace from (%s) to (%s)"), *StartLocation.ToString(), *EndLocation.ToString());
    }
}
