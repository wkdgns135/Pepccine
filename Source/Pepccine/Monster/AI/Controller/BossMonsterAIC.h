#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"  // BehaviorTree 참조 위해 추가
#include "BossMonsterAIC.generated.h"

class AActor;
class UBlackboardComponent;


UCLASS()
class PEPCCINE_API ABossMonsterAIC : public AAIController
{
	GENERATED_BODY()

public:
	ABossMonsterAIC();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	
	void RunAI();
	void StopAI();
	void SetTarget(AActor* Target);
	void SetIsInAttackRange(bool InRange);
	void SetCanBerserk(bool bCanBerserk);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	void InitializeBehaviorTree(APawn* InPawn);
};
