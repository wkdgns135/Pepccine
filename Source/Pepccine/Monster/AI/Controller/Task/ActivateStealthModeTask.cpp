#include "Monster/AI/Controller/Task/ActivateStealthModeTask.h"
#include "Monster/Component/StealthModeSkillComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UActivateStealthModeTask::UActivateStealthModeTask()
{
    NodeName = TEXT("Activate Stealth Mode");
}

EBTNodeResult::Type UActivateStealthModeTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // BlackBoard에서 타겟을 가져오기
    AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

    // 타겟이 존재하고, StealthMode 컴포넌트가 있을 경우
    if (TargetActor)
    {
        // OwnerComp의 소유자가 AIController인지 확인
        AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
        if (AIController)
        {
            APawn* OwnerPawn = AIController->GetPawn();  // AIController에서 Pawn을 가져오기.
            if (OwnerPawn)
            {
                UStealthModeSkillComponent* StealthModeComp = OwnerPawn->FindComponentByClass<UStealthModeSkillComponent>();
                if (StealthModeComp)
                {
                    // 은신 모드 실행
                    StealthModeComp->ActivateSkill();
                    return EBTNodeResult::Succeeded;
                }
            }
        }
    }

    return EBTNodeResult::Failed;
}
