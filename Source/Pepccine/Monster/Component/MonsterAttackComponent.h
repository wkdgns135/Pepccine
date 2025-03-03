#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Components/BattleComponent.h"
#include "MonsterAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UMonsterAttackComponent : public UBattleComponent
{
	GENERATED_BODY()

public:	
	UMonsterAttackComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void PerformAttack();
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void AttackTrace();
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void PlayTransitionMontage();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* AttackTransitionMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackRange = 60.0f;

	virtual void BeginPlay() override;

private:
	
};
