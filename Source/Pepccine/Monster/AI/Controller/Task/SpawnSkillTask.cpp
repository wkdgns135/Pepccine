#include "Monster/AI/Controller/Task/SpawnSkillTask.h"
#include "Monster/Component/SpawnSkillComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

USpawnSkillTask::USpawnSkillTask()
{
    NodeName = TEXT("Spawn Skill");
}

EBTNodeResult::Type USpawnSkillTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // BlackBoard에서 타겟을 가져오기
    AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

    // 타겟이 존재하고, 스폰 스킬 컴포넌트가 있을 경우
    if (TargetActor)
    {
        // OwnerComp의 소유자가 AIController인지 확인
        AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
        if (AIController)
        {
            APawn* OwnerPawn = AIController->GetPawn();  // AIController에서 Pawn을 가져오기.
            if (OwnerPawn)
            {
                USpawnSkillComponent* SpawnSkillComp = OwnerPawn->FindComponentByClass<USpawnSkillComponent>();
                if (SpawnSkillComp)
                {
                    // 점프 공격 실행
                    SpawnSkillComp->ActivateSkill();
                    return EBTNodeResult::Succeeded;
                }
            }
        }
    }

    return EBTNodeResult::Failed;
}