#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AttackTransitionTask.generated.h"

UCLASS()
class PEPCCINE_API UAttackTransitionTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UAttackTransitionTask();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
