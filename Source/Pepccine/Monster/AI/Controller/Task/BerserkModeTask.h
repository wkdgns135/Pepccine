#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BerserkModeTask.generated.h"

UCLASS()
class PEPCCINE_API UBerserkModeTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBerserkModeTask();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
