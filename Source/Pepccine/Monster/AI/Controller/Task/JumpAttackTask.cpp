#include "Monster/AI/Controller/Task/JumpAttackTask.h"
#include "Monster/Component/JumpAttackComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UJumpAttackTask::UJumpAttackTask()
{
    NodeName = TEXT("Jump Attack");
}

EBTNodeResult::Type UJumpAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // BlackBoard에서 타겟을 가져오기
    AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

    // 타겟이 존재하고, 점프 공격 컴포넌트가 있을 경우
    if (TargetActor)
    {
        // OwnerComp의 소유자가 AIController인지 확인
        AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
        if (AIController)
        {
            APawn* OwnerPawn = AIController->GetPawn();  // AIController에서 Pawn을 가져오기.
            if (OwnerPawn)
            {
                UJumpAttackComponent* JumpAttackComp = OwnerPawn->FindComponentByClass<UJumpAttackComponent>();
                if (JumpAttackComp)
                {
                    // 점프 공격 실행
                    JumpAttackComp->ActivateSkill();
                    return EBTNodeResult::Succeeded;
                }
            }
        }
    }

    return EBTNodeResult::Failed;
}
