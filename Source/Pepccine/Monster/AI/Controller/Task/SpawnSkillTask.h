#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "SpawnSkillTask.generated.h"

UCLASS()
class PEPCCINE_API USpawnSkillTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	USpawnSkillTask();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
