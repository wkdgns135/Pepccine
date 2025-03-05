#include "Monster/AI/Controller/NormalMonsterAIC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Monster/Class/BaseMonster.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ANormalMonsterAIC::ANormalMonsterAIC()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ANormalMonsterAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	InitializeBehaviorTree(InPawn);
}

void ANormalMonsterAIC::BeginPlay()
{
	Super::BeginPlay();
}

void ANormalMonsterAIC::InitializeBehaviorTree(APawn* InPawn)
{
    ABaseMonster* BaseMonster = Cast<ABaseMonster>(InPawn);
    UBehaviorTree* BehaviorTree = BaseMonster->GetBehaviorTree();

    if (BaseMonster && BehaviorTree)
    {
        if (UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent))
        {
            RunBehaviorTree(BehaviorTree);

            // 1. 맵이 바로 전환될때 타이머 람다 함수 내에서 예외 발생 (GetWorld()가 nullptr)
            // 2. 람다 함수 자체가 레벨이 전환될때 캡쳐로 넘긴 this 포인터의 유효성을 검사하지 못함.
            GetWorldTimerManager().SetTimerForNextTick(this, &ANormalMonsterAIC::SetTarget);
            
        }
    }
}

void ANormalMonsterAIC::SetTarget()
{
    if (UWorld *World = GetWorld())
    {
        if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0))
        {
            if (BlackboardComponent)
            {
                BlackboardComponent->SetValueAsObject("TargetActor", PlayerCharacter);
            }
        }
    }
}
