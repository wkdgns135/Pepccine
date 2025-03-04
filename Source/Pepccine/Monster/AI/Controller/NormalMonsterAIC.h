#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "NormalMonsterAIC.generated.h"


UCLASS()
class PEPCCINE_API ANormalMonsterAIC : public AAIController
{
	GENERATED_BODY()
	
public:
	ANormalMonsterAIC();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	void SetTarget();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

private:
	void InitializeBehaviorTree(APawn* InPawn);
};
