#include "Monster/AI/Controller/BossPumpkinHulkAIC.h"
#include "BehaviorTree/BlackboardComponent.h"

ABossPumpkinHulkAIC::ABossPumpkinHulkAIC()
{
}

void ABossPumpkinHulkAIC::BeginPlay()
{
    Super::BeginPlay();
}

void ABossPumpkinHulkAIC::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void ABossPumpkinHulkAIC::SetIsInJumpAttackRange(bool InRange)
{
    if (BlackboardComponent)
    {
        BlackboardComponent->SetValueAsBool("IsInJumpAttackRange", InRange);
    }
}