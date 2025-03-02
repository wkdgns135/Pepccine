#include "Monster/AI/Controller/Task/AttackTransitionTask.h"
#include "Monster/Component/MonsterAttackComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UAttackTransitionTask::UAttackTransitionTask()
{
    NodeName = TEXT("Attack Transition");
}

EBTNodeResult::Type UAttackTransitionTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

    if (TargetActor)
    {
        AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
        if (AIController)
        {
            APawn* OwnerPawn = AIController->GetPawn();
            if (OwnerPawn)
            {
                UMonsterAttackComponent* AttackComp = OwnerPawn->FindComponentByClass<UMonsterAttackComponent>();
                if (AttackComp)
                {
                    // Montage ½ÇÇà
                    AttackComp->PlayTransitionMontage();
                    return EBTNodeResult::Succeeded;
                }
            }
        }
    }

    return EBTNodeResult::Failed;
}
