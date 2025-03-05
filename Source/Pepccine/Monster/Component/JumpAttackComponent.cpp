#include "Monster/Component/JumpAttackComponent.h"
#include "Character/Components/BattleComponent.h"
#include "Character/Player/PepCharacter.h"
#include "GameFramework/Character.h"

UJumpAttackComponent::UJumpAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UJumpAttackComponent::ActivateSkill()
{
    if (!IsCooldownOver())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cooldown not over yet!"));
        return;
    }

    StartCooldown();
    PlaySkillMontage();
}

void UJumpAttackComponent::LaunchMonster()
{
    ACharacter* OwnerMonster = Cast<ACharacter>(GetOwner());
    if (OwnerMonster)
    {

        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            UE_LOG(LogTemp, Warning, TEXT("PlayerController found"));

            APawn* PlayerPawn = PlayerController->GetPawn();
            if (PlayerPawn)
            {
                // 플레이어와 몬스터의 현재 위치를 계산
                FVector PlayerLocation = PlayerPawn->GetActorLocation();
                FVector MonsterLocation = OwnerMonster->GetActorLocation();

                // 목표 위치까지의 차이 계산
                FVector DirectionToPlayer = (PlayerLocation - MonsterLocation).GetSafeNormal();

                // 점프할 방향과 세기 계산
                FVector LaunchDirection = DirectionToPlayer * JumpPower;

                // 점프 높이도 설정 (Z축)
                LaunchDirection.Z = JumpHeight;

                // 점프 적용
                OwnerMonster->LaunchCharacter(LaunchDirection, true, true);

                UE_LOG(LogTemp, Warning, TEXT("Jump launched towards player at: %s"), *PlayerLocation.ToString());
            }
        }
    }
}

void UJumpAttackComponent::SkillTrace()
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("JumpAttackComponent: No Owner found!"));
        return;
    }

    FVector Center = Owner->GetActorLocation(); // 몬스터 중심
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner); // 자기 자신 제외


    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        Center, // 시작 위치 (몬스터 중심)
        Center, // 종료 위치 (같은 위치에서 반경만 체크)
        FQuat::Identity, // 회전 없음
        ECC_GameTraceChannel1, // 충돌 채널
        FCollisionShape::MakeSphere(AttackRadius), // 범위 (구체)
        QueryParams
    );

    DrawDebugSphere(GetWorld(), Center, AttackRadius, 12, FColor::Green, false, 2.0f);

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());

            DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);

            UBattleComponent* TargetBattleComponent = HitActor->FindComponentByClass<UBattleComponent>();
            if (TargetBattleComponent)
            {
                TargetBattleComponent->SendHitResult(HitActor, Damage, HitResult, EMonsterSkill::JumpAttack);
            }
        }
    }
}

