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

    if (AttackComponent)
    {
        AttackComponent->OnCharacterHited.AddDynamic(this, &ABaseMonster::OnHitReceived);
    }

    UE_LOG(LogTemp, Warning, TEXT("Monster Spawned!"));
}

void ABaseMonster::OnHitReceived(AActor* DamageCauser, float DamageAmount, const FHitResult& HitResult)
{
    UE_LOG(LogTemp, Warning, TEXT("Monster hit! Damage: %f, Hit Location: %s"), DamageAmount, *HitResult.Location.ToString());
    if (StatComponent)
    {
        StatComponent->DecreaseHealth(DamageAmount);

        FVector HitDirection = DamageCauser ? (GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal() : FVector::ZeroVector;

        if (HitReactionComponent)
        {
            HitReactionComponent->HandleHitReaction(DamageAmount);
        }

        if (StatComponent->IsDead())
        {
            UE_LOG(LogTemp, Warning, TEXT("Monster %s has died!"), *GetName());
            Die();
        }
    }
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

    SetLifeSpan(5.0f);
}
