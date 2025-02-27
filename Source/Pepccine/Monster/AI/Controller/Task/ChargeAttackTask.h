#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ChargeAttackTask.generated.h"

UCLASS()
class PEPCCINE_API UChargeAttackTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UChargeAttackTask();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
