#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UMonsterAttackComponent : public UActorComponent
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

	virtual void BeginPlay() override;

private:
	void ApplyDamageToTarget(AActor* Target, float DamageAmount);
	
};
