#include "Monster/AI/Controller/Task/CustomMoveToTask.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

UCustomMoveToTask::UCustomMoveToTask()
{
    NodeName = TEXT("Custom Move To (With Acceleration)");
    bNotifyTick = true;  // Tick 함수 활성화
}

EBTNodeResult::Type UCustomMoveToTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    AICharacter = Cast<ACharacter>(AIController->GetPawn());
    if (!AICharacter)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    // Blackboard에서 Target을 가져옴 (Object 또는 Actor)
    UObject* TargetObject = BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey());
    TargetActor = Cast<AActor>(TargetObject);

    if (!TargetActor)
    {
        return EBTNodeResult::Failed;
    }

    // 네비게이션 경로 찾기
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem)
    {
        return EBTNodeResult::Failed;
    }

    UNavigationPath* NavPath = NavSystem->FindPathToActorSynchronously(GetWorld(), AICharacter->GetActorLocation(), TargetActor);

    if (!NavPath || NavPath->PathPoints.Num() == 0)
    {
        return EBTNodeResult::Failed;
    }

    // 경로 저장
    PathPoints = NavPath->PathPoints;
    CurrentPathIndex = 0;

    // 가속도 설정
    AICharacter->GetCharacterMovement()->MaxAcceleration = 2048.0f;
    AICharacter->GetCharacterMovement()->bRequestedMoveUseAcceleration = true;

    return EBTNodeResult::InProgress;
}

void UCustomMoveToTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (!AICharacter || !TargetActor || PathPoints.Num() == 0)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // 현재 목표 지점
    FVector CurrentTarget = PathPoints[CurrentPathIndex];

    // 현재 위치에서 목표 지점까지의 거리 계산
    float Distance = FVector::Dist(AICharacter->GetActorLocation(), CurrentTarget);

    // 도착했으면 다음 경로로 이동
    if (Distance < 100.0f)
    {
        CurrentPathIndex++;

        // 마지막 지점 도착 시 성공 처리
        if (CurrentPathIndex >= PathPoints.Num())
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return;
        }

        CurrentTarget = PathPoints[CurrentPathIndex];
    }

    // 목표 지점으로 이동 (가속도 반영)
    FVector Direction = (CurrentTarget - AICharacter->GetActorLocation()).GetSafeNormal();
    FVector NewVelocity = Direction * AICharacter->GetCharacterMovement()->MaxWalkSpeed;

    AICharacter->GetCharacterMovement()->Velocity = FMath::VInterpTo(AICharacter->GetCharacterMovement()->Velocity, NewVelocity, DeltaSeconds, 2.0f);
}
