#include "Monster/Component/MonsterAttackComponent.h"
#include "Monster/Component/MonsterStatComponent.h"
#include "Monster/Class/BaseMonster.h"
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
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Owner is NOT an ACharacter!"));
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("%s is attacking!"), *OwnerCharacter->GetName());

    if (AttackMontage)
    {
        USkeletalMeshComponent* MeshComp = OwnerCharacter->FindComponentByClass<USkeletalMeshComponent>();
        UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
        if (MeshComp && AnimInstance)
        {
            int32 JumpSection = FMath::RandRange(0, AttackMontage->CompositeSections.Num());
            FName SectionName = FName(*FString::Printf(TEXT("%d"), JumpSection));
            AnimInstance->Montage_Play(AttackMontage);
            AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
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

// AttackTrace에 매개변수로 SkillType, AttackType(원거리, 근거리) 넣어야 합니다.
void UMonsterAttackComponent::AttackTrace()
{
    ABaseMonster* OwnerMonster = Cast<ABaseMonster>(GetOwner());
    if (!OwnerMonster)
    {
        UE_LOG(LogTemp, Warning, TEXT("Owner is NOT an ABaseMonster!"));
        return;
    }

    float CapsuleRadius = (OwnerMonster->GetMonsterType() == EMonsterType::LongRange) ? 5.0f : 20.0f;
    float CapsuleHalfHeight = (OwnerMonster->GetMonsterType() == EMonsterType::LongRange) ? AttackRange * 0.5f : 30.0f;
    EMonsterSkill AttackType = (OwnerMonster->GetMonsterType() == EMonsterType::LongRange) ? EMonsterSkill::GunShot : EMonsterSkill::None;

    ExecuteTrace(OwnerMonster, AttackRange, CapsuleRadius, CapsuleHalfHeight, AttackType);
}

void UMonsterAttackComponent::ExecuteTrace(ABaseMonster* OwnerMonster, float Range, float CapsuleRadius, float CapsuleHalfHeight, EMonsterSkill AttackType)
{
    FVector StartLocation = OwnerMonster->GetActorLocation();
    FVector ForwardVector = OwnerMonster->GetActorForwardVector();
    FVector EndLocation = StartLocation + (ForwardVector * Range);

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(OwnerMonster);

    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        FQuat(FRotator(90, 0, 0)),
        ECC_GameTraceChannel1,
        FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
        CollisionParams
    );

    if (bHit)
    {
        FVector HitLocation = HitResult.Location;
        DrawDebugPoint(GetWorld(), HitLocation, 10.0f, FColor::Red, false, 2.0f);

        UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());

        APepCharacter* Player = Cast<APepCharacter>(HitResult.GetActor());
        if (Player)
        {
            UMonsterStatComponent* StatComp = OwnerMonster->FindComponentByClass<UMonsterStatComponent>();
            if (StatComp)
            {
                SendHitResult(Player, StatComp->Attack, HitResult, AttackType);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("StatComp is nullptr in ExecuteTrace!"));
            }
            
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Missed! No target hit."));
    }
}
