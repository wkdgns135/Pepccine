#include "Monster/Component/ChargeAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Components/BattleComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Animation/AnimInstance.h"
#include "Character/Player/PepCharacter.h"
#include "DrawDebugHelpers.h"
#include "Monster/Component/MonsterStatComponent.h"

UChargeAttackComponent::UChargeAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = true;  // Tick 활성화
    bIsCharging = false;
    ChargeStartTime = 0.0f;
    OriginalSpeed = 0.0f;
}

void UChargeAttackComponent::BeginPlay()
{
    Super::BeginPlay();
    SetSkillDamage();
}

void UChargeAttackComponent::ActivateSkill()
{
    if (!IsCooldownOver())
    {
        return;
    }

    StartCooldown();
    PlaySkillMontage();  // 기존 charge 애니메이션 재생
    StartCharge();
}

void UChargeAttackComponent::StartCharge()
{
    ACharacter* OwnerMonster = Cast<ACharacter>(GetOwner());
    if (OwnerMonster)
    {
        // 돌진 시작 시 속도 설정
        OriginalSpeed = OwnerMonster->GetCharacterMovement()->MaxWalkSpeed;  // 기존 속도 저장
        OwnerMonster->GetCharacterMovement()->MaxWalkSpeed = ChargeSpeed;

        bIsCharging = true;
        ChargeStartTime = GetWorld()->GetTimeSeconds();

        // 일정 시간 후 돌진 중지
        GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &UChargeAttackComponent::StopCharge, ChargeDuration, false);
    }
}

void UChargeAttackComponent::StopCharge()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
        OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
    }

    PlayEndAnimation();
    bIsCharging = false;

    DamagedActors.Empty(); // 공격 대상 초기화
}

void UChargeAttackComponent::PlayEndAnimation()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
        if (AnimInstance && ChargeSkillEndMontage)  // EndAnimationMontage는 에디터에서 설정한 종료 애니메이션 몽타주
        {
            AnimInstance->Montage_Play(ChargeSkillEndMontage);
        }
    }
}

void UChargeAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsCharging)
    {
        ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
        if (OwnerCharacter)
        {
            // 돌진 중 계속 이동 처리
            FVector ForwardDirection = OwnerCharacter->GetActorForwardVector();
            OwnerCharacter->AddMovementInput(ForwardDirection, 1.0f);

            ChargeTrace();
        }
    }
}

void UChargeAttackComponent::ChargeTrace()
{
    const AActor* Owner = GetOwner();

    if (!Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("ChargeAttackComponent: No Owner found!"));
        return;
    }

    FVector Start = Owner->GetActorLocation();
    TArray<FHitResult> HitResults;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);

    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

    bool bHit = GetWorld()->SweepMultiByObjectType(
        HitResults,
        Start,
        Start,
        FQuat::Identity,
        ObjectQueryParams,
        FCollisionShape::MakeSphere(AttackRadius),
        QueryParams
    );

    // 트레이스 시작 지점과 끝 지점을 구체로 표시 (파란색)
    DrawDebugSphere(GetWorld(), Start, AttackRadius, 12, FColor::Blue, false, 0.1f);

    if (bHit)
    {
        for (const FHitResult& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            if (HitActor)
            {
                UE_LOG(LogTemp, Log, TEXT("Charge Hit: %s"), *HitActor->GetName());

                // 충돌 지점을 빨간색 점으로 표시
                DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);

                OnHit(HitActor, Hit);
                return;
            }
        }
    }
}


void UChargeAttackComponent::OnHit(AActor* HitActor, const FHitResult& Hit)
{
    if (!HitActor || DamagedActors.Contains(HitActor)) // 이미 공격한 대상이면 리턴
    {
        return;
    }

    if (APepCharacter* Player = Cast<APepCharacter>(HitActor))
    {
        UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());

        DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);

        if (UBattleComponent* TargetBattleComponent = Player->FindComponentByClass<UBattleComponent>())
        {
            TargetBattleComponent->SendHitResult(Player, SkillDamage, Hit, EMonsterSkill::Charge);

            DamagedActors.Add(HitActor); // 공격한 대상 기록
        }
    }
}

void UChargeAttackComponent::SetSkillDamage()
{
    ACharacter* OwnerMonster = Cast<ACharacter>(GetOwner());
    UMonsterStatComponent* MonsterStatComponent = OwnerMonster->FindComponentByClass<UMonsterStatComponent>();
    SkillDamage = MonsterStatComponent->Attack * DamageMultiplier;
}