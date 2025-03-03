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
    UBehaviorTree* BehaviorTree = BaseMonster->GetBehaviorTree();
    if (BaseMonster && BehaviorTree)
    {
        if (UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent))
        {
            RunBehaviorTree(BehaviorTree);

            // 플레이어를 기본 타겟으로 설정
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
                {
                    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
                    if (PlayerCharacter)
                    {
                        SetTarget(PlayerCharacter);
                    }
                }, 0.1f, false);
        }
    }
}

void ANormalMonsterAIC::SetTarget(AActor* Target)
{
    if (BlackboardComponent)
    {
        BlackboardComponent->SetValueAsObject("TargetActor", Target);
    }
}