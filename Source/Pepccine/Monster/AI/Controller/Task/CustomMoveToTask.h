#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "CustomMoveToTask.generated.h"

UCLASS()
class PEPCCINE_API UCustomMoveToTask : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
    UCustomMoveToTask();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    AAIController* AIController;
    ACharacter* AICharacter;
    AActor* TargetActor;
    FVector TargetLocation;
    TArray<FVector> PathPoints;
    int32 CurrentPathIndex;
};