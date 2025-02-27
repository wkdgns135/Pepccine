#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "JumpAttackTask.generated.h"

UCLASS()
class PEPCCINE_API UJumpAttackTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UJumpAttackTask();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};