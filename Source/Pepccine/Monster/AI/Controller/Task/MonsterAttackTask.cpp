#include "Monster/AI/Controller/Task/MonsterAttackTask.h"
#include "Monster/Component/MonsterAttackComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UMonsterAttackTask::UMonsterAttackTask()
{
    NodeName = TEXT("Monster Attack");
}

EBTNodeResult::Type UMonsterAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // BlackBoard에서 타겟을 가져오기
    AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

    // 타겟이 존재하고, 몬스터의 AttackComponent가 있는 경우
    if (TargetActor)
    {
        // OwnerComp의 소유자가 AIController인지 확인
        AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
        if (AIController)
        {
            APawn* OwnerPawn = AIController->GetPawn();  // AIController에서 Pawn을 가져오기.
            if (OwnerPawn)
            {
                UMonsterAttackComponent* AttackComp = OwnerPawn->FindComponentByClass<UMonsterAttackComponent>();
                if (AttackComp)
                {
                    // 공격 실행
                    AttackComp->PerformAttack();
                    return EBTNodeResult::Succeeded;
                }
            }
        }
    }

    return EBTNodeResult::Failed;
}
