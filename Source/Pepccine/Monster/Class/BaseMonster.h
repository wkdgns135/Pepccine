#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster/Widget/MonsterHealthWidget.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BaseMonster.generated.h"

class UWidgetComponent;
class UMonsterStatComponent;
class UMonsterAttackComponent;
class UHitReactionComponent;
class UAudioComponent;
class UMonsterHealthWidget;
class UPepccineItemSpawnWeightData;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	UWidgetComponent* HealthBarWidgetComp;
	UPROPERTY()
	UMonsterHealthWidget* HealthInstance;
	
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);
	void InitializeHealthBar();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere)
	USoundBase* BaseSound;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMonsterAttackComponent* AttackComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMonsterStatComponent* StatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHitReactionComponent* HitReactionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster")
	EMonsterType MonsterType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	UPepccineItemSpawnWeightData* SpawnWeightData;

private:

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void OnHitReceived(AActor* DamageCauser, float DamageAmount, const FHitResult& HitResult, EMonsterSkill SkillType);
	void Die();
};
