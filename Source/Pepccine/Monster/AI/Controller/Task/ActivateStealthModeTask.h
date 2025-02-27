#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ActivateStealthModeTask.generated.h"

UCLASS()
class PEPCCINE_API UActivateStealthModeTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UActivateStealthModeTask();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
