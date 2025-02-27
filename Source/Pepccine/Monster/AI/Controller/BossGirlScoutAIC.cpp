#include "Monster/AI/Controller/BossGirlScoutAIC.h"
#include "BehaviorTree/BlackboardComponent.h"

ABossGirlScoutAIC::ABossGirlScoutAIC()
{
}

void ABossGirlScoutAIC::BeginPlay()
{
    Super::BeginPlay();
}

void ABossGirlScoutAIC::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void ABossGirlScoutAIC::SetIsInJumpAttackRange(bool InRange)
{
    if (BlackboardComponent)
    {
        BlackboardComponent->SetValueAsBool("IsInJumpAttackRange", InRange);
    }
}