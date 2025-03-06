#include "Monster/AI/Controller/NormalMonsterAIC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Monster/Class/BaseMonster.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ANormalMonsterAIC::ANormalMonsterAIC()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ANormalMonsterAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	InitializeBehaviorTree(InPawn);
}

void ANormalMonsterAIC::BeginPlay()
{
	Super::BeginPlay();
}

void ANormalMonsterAIC::InitializeBehaviorTree(APawn* InPawn)
{
    ABaseMonster* BaseMonster = Cast<ABaseMonster>(InPawn);
    BehaviorTree = BaseMonster->GetBehaviorTree();

    if (BaseMonster && BehaviorTree)
    {
        if (UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent))
        {
            RunBehaviorTree(BehaviorTree);
            GetWorldTimerManager().SetTimerForNextTick(this, &ANormalMonsterAIC::SetTarget);
            
        }
    }
}

void ANormalMonsterAIC::SetTarget()
{
    if (UWorld *World = GetWorld())
    {
        if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0))
        {
            if (BlackboardComponent)
            {
                BlackboardComponent->SetValueAsObject("TargetActor", PlayerCharacter);
            }
        }
    }
}
