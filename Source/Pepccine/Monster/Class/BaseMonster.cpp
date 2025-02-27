#include "Monster/Class/BaseMonster.h"
#include "Monster/Component/MonsterStatComponent.h"
#include "Monster/Component/MonsterAttackComponent.h"
#include "Monster/Component/HitReactionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseMonster::ABaseMonster()
{
    StatComponent = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("StatComponent"));
    AttackComponent = CreateDefaultSubobject<UMonsterAttackComponent>(TEXT("AttackComponent"));
    HitReactionComponent = CreateDefaultSubobject<UHitReactionComponent>(TEXT("HitReactionComponent"));
}

void ABaseMonster::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("Monster Spawned!"));
}

float ABaseMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // 로그 추가: 데미지를 받은 액터, 데미지 양, 데미지 이벤트 정보 확인
    UE_LOG(LogTemp, Warning, TEXT("Monster %s took %f damage!"), *GetName(), DamageAmount);

    if (StatComponent)
    {
        StatComponent->DecreaseHealth(DamageAmount);

        // 피격 방향을 계산하여 전달
        FVector HitDirection = DamageCauser ? (GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal() : FVector::ZeroVector;

        if (HitReactionComponent)
        {
            HitReactionComponent->HandleHitReaction(DamageAmount);
        }

        if (StatComponent->IsDead())
        {
            // 몬스터 죽었을 때 로그 
            UE_LOG(LogTemp, Warning, TEXT("Monster %s has died!"), *GetName());
            Die();
        }
    }

    return DamageAmount;
}


void ABaseMonster::Die()
{
    UE_LOG(LogTemp, Warning, TEXT("Monster Die!"));

    // 물리적 처리 (Ragdoll 활성화)
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
    GetMesh()->Stop();
    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->WakeAllRigidBodies();
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    GetCharacterMovement()->DisableMovement();
    GetCharacterMovement()->StopMovementImmediately();

    // 수명 설정 (지정 시간 후 삭제)
    SetLifeSpan(10.0f);
}
