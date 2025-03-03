#include "Monster/AI/Controller/BossMonsterAIC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ABossMonsterAIC::ABossMonsterAIC()
{
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ABossMonsterAIC::BeginPlay()
{
    Super::BeginPlay();

}

void ABossMonsterAIC::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    InitializeBehaviorTree(InPawn);
}

void ABossMonsterAIC::InitializeBehaviorTree(APawn* InPawn)
{
    if (BehaviorTree)
    {
        if (UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent))
        {
            RunBehaviorTree(BehaviorTree);

            // 플레이어를 기본 타겟으로 설정
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
                {
                    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
                    if (PlayerCharacter)
                    {
                        SetTarget(PlayerCharacter);
                    }
                }, 0.1f, false);
        }
    }
}

void ABossMonsterAIC::RunAI()
{
    if (BehaviorTree && BrainComponent && !BrainComponent->IsRunning())
    {
        UBehaviorTreeComponent* BehaviorTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
        if (BehaviorTreeComp)
        {
            BehaviorTreeComp->StartTree(*BehaviorTree);
        }
    }
}

void ABossMonsterAIC::StopAI()
{
    if (BrainComponent)
    {
        BrainComponent->StopLogic("Stopped by AI Controller");
    }
}

void ABossMonsterAIC::SetTarget(AActor* Target)
{
    if (BlackboardComponent)
    {
        BlackboardComponent->SetValueAsObject("TargetActor", Target);
    }
}

void ABossMonsterAIC::SetIsInAttackRange(bool InRange)
{
    if (BlackboardComponent)
    {
        BlackboardComponent->SetValueAsBool("IsInAttackRange", InRange);
    }
}