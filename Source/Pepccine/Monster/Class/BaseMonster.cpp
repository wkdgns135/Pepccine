#include "Monster/Class/BaseMonster.h"

#include "PepccineGameState.h"
#include "Monster/Component/MonsterStatComponent.h"
#include "Monster/Component/MonsterAttackComponent.h"
#include "Monster/Component/HitReactionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Room/Controller/BaseRoomController.h"

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

    // �α� �߰�: �������� ���� ����, ������ ��, ������ �̺�Ʈ ���� Ȯ��
    UE_LOG(LogTemp, Warning, TEXT("Monster %s took %f damage!"), *GetName(), DamageAmount);

    if (StatComponent)
    {
        StatComponent->DecreaseHealth(DamageAmount);

        // �ǰ� ������ ����Ͽ� ����
        FVector HitDirection = DamageCauser ? (GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal() : FVector::ZeroVector;

        if (HitReactionComponent)
        {
            HitReactionComponent->HandleHitReaction(DamageAmount);
        }

        if (StatComponent->IsDead())
        {
            // ���� �׾��� �� �α� 
            UE_LOG(LogTemp, Warning, TEXT("Monster %s has died!"), *GetName());
            Die();
        }
    }

    return DamageAmount;
}


void ABaseMonster::Die()
{
    UE_LOG(LogTemp, Warning, TEXT("Monster Die!"));

    // ������ ó�� (Ragdoll Ȱ��ȭ)
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
    GetMesh()->Stop();
    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->WakeAllRigidBodies();
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    GetCharacterMovement()->DisableMovement();
    GetCharacterMovement()->StopMovementImmediately();

    if (APepccineGameState *PepccineGameState = Cast<APepccineGameState>(GetWorld()->GetGameState()))
    {
        if (ABaseRoomController *RoomController = PepccineGameState->GetRoomController())
        {
            RoomController->DecreaseMonsterCount();
        }
    }
    
    // ���� ���� (���� �ð� �� ����)
    SetLifeSpan(10.0f);
}
