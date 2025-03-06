#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Components/BattleComponent.h"
#include "MonsterAttackComponent.generated.h"

class ABaseMonster;

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
	FORCEINLINE float GetAttackRange() { return AttackRange; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* AttackTransitionMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackRange = 1000.0f;

	virtual void BeginPlay() override;

private:
	void ExecuteTrace(ABaseMonster* OwnerMonster, float Range, float CapsuleRadius, float CapsuleHalfHeight, EMonsterSkill AttackType);
};
