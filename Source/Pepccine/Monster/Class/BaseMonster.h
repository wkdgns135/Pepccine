#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BaseMonster.generated.h"


class UMonsterStatComponent;
class UMonsterAttackComponent;
class UHitReactionComponent;

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	None        UMETA(DisplayName = "None"),
	CloseRange  UMETA(DisplayName = "CloseRange"),
	LongRange   UMETA(DisplayName = "LongRange"),
};

UCLASS()
class PEPCCINE_API ABaseMonster : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseMonster();

	FORCEINLINE EMonsterType GetMonsterType() const { return MonsterType; }
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster")
	EMonsterType MonsterType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMonsterAttackComponent* AttackComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMonsterStatComponent* StatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHitReactionComponent* HitReactionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	virtual void BeginPlay() override;

private:

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void OnHitReceived(AActor* DamageCauser, float DamageAmount, const FHitResult& HitResult);
	void Die();
};
