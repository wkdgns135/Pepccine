#include "Monster/AI/Controller/Task/ChargeAttackTask.h"
#include "Monster/Component/ChargeAttackComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UChargeAttackTask::UChargeAttackTask()
{
    NodeName = TEXT("Charge Attack");
}

EBTNodeResult::Type UChargeAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // BlackBoard에서 타겟을 가져오기
    AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

    // 타겟이 존재하고, ChargeAttackComponent가 있을 경우
    if (TargetActor)
    {
        // OwnerComp의 소유자가 AIController인지 확인
        AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
        if (AIController)
        {
            APawn* OwnerPawn = AIController->GetPawn();  // AIController에서 Pawn을 가져오기.
            if (OwnerPawn)
            {
                UChargeAttackComponent* ChargeAttackComp = OwnerPawn->FindComponentByClass<UChargeAttackComponent>();
                if (ChargeAttackComp)
                {
                    // 돌진 공격 실행
                    ChargeAttackComp->ActivateSkill();
                    return EBTNodeResult::Succeeded;
                }
            }
        }
    }

    return EBTNodeResult::Failed;
}
