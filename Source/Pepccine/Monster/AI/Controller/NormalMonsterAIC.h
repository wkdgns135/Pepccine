#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NormalMonsterAIC.generated.h"


UCLASS()
class PEPCCINE_API ANormalMonsterAIC : public AAIController
{
	GENERATED_BODY()
	
public:
	ANormalMonsterAIC();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComponent;


private:
	void InitializeBehaviorTree(APawn* InPawn);
	void SetTarget(AActor* Target);
};
