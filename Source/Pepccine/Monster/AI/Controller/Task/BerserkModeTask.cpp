#include "Monster/AI/Controller/Task/BerserkModeTask.h"
#include "Monster/Component/BerserkModeComponent.h"
#include "AIController.h"

UBerserkModeTask::UBerserkModeTask()
{
    NodeName = TEXT("Berserk Mode Change");
}

EBTNodeResult::Type UBerserkModeTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // OwnerComp의 소유자가 AIController인지 확인
    AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
    if (AIController)
    {
        APawn* OwnerPawn = AIController->GetPawn();  // AIController에서 Pawn을 가져오기.
        if (OwnerPawn)
        {
            UBerserkModeComponent* BerserkModeComp = OwnerPawn->FindComponentByClass<UBerserkModeComponent>();
            if (BerserkModeComp)
            {
                // 버서크 모드 실행
                BerserkModeComp->EnterBerserkMode();
                return EBTNodeResult::Succeeded;
            }
        }
    }
    return EBTNodeResult::Failed;
}