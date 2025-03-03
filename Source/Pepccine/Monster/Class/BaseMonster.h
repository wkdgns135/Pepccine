#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster/Widget/MonsterHealthWidget.h"
#include "BaseMonster.generated.h"

class UWidgetComponent;
class UMonsterStatComponent;
class UMonsterAttackComponent;
class UHitReactionComponent;
class UMonsterHealthWidget;

UCLASS()
class PEPCCINE_API ABaseMonster : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseMonster();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	UWidgetComponent* HealthBarWidgetComp;
	
	UPROPERTY()
	UMonsterHealthWidget* HealthInstance;
	
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);
	void InitializeHealthBar();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMonsterAttackComponent* AttackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMonsterStatComponent* StatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHitReactionComponent* HitReactionComponent;

private:

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void OnHitReceived(AActor* DamageCauser, float DamageAmount, const FHitResult& HitResult);
	void Die();
};
