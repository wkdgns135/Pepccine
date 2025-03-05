#include "Monster/Component/JumpAttackComponent.h"
#include "Monster/Component/MonsterStatComponent.h"
#include "Character/Components/BattleComponent.h"
#include "Character/Player/PepCharacter.h"
#include "GameFramework/Character.h"

UJumpAttackComponent::UJumpAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UJumpAttackComponent::BeginPlay()
{
    Super::BeginPlay();

    SetSkillDamage();
}


void UJumpAttackComponent::SetSkillDamage()
{
    ACharacter* OwnerMonster = Cast<ACharacter>(GetOwner());
    UMonsterStatComponent* MonsterStatComponent = OwnerMonster->FindComponentByClass<UMonsterStatComponent>();
    SkillDamage = MonsterStatComponent->Attack * DamageMultiplier;
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
                FVector PlayerLocation = PlayerPawn->GetActorLocation();
                FVector MonsterLocation = OwnerMonster->GetActorLocation();

                FVector DirectionToPlayer = (PlayerLocation - MonsterLocation).GetSafeNormal();

                FVector LaunchDirection = DirectionToPlayer * JumpPower;

                LaunchDirection.Z = JumpHeight;
                OwnerMonster->LaunchCharacter(LaunchDirection, true, true);

                UE_LOG(LogTemp, Warning, TEXT("Jump launched towards player at: %s"), *PlayerLocation.ToString());
            }
        }
    }
}

void UJumpAttackComponent::SkillTrace()
{
    const AActor* Owner = GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("JumpAttackComponent: No Owner found!"));
        return;
    }

    FVector Center = Owner->GetActorLocation();
    TArray<FHitResult> HitResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);

    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

    bool bHit = GetWorld()->SweepMultiByObjectType(
        HitResults,
        Center,
        Center,
        FQuat::Identity,
        ObjectQueryParams,
        FCollisionShape::MakeSphere(AttackRadius),
        QueryParams
    );

    DrawDebugSphere(GetWorld(), Center, AttackRadius, 12, FColor::Green, false, 2.0f);
    
    if (bHit)
    {
        for (const FHitResult& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            if (APepCharacter* Player = Cast<APepCharacter>(HitActor))
            {
                UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());

                DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);

                if (UBattleComponent* TargetBattleComponent = Player->FindComponentByClass<UBattleComponent>())
                {
                    TargetBattleComponent->SendHitResult(Player, SkillDamage, Hit, EMonsterSkill::JumpAttack);
                    return;
                }
            }
        }
    }
}