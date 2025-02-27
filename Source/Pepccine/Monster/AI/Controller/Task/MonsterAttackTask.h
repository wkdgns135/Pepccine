#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MonsterAttackTask.generated.h"

UCLASS()
class PEPCCINE_API UMonsterAttackTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UMonsterAttackTask();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};