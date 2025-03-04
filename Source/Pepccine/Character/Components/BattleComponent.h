#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Monster/Data/MonsterSkill.h"
#include "BattleComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCharacterHited, AActor*, DamageCauser, float, DamageAmount, const FHitResult&, HitResult, EMonsterSkill, SkillType);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PEPCCINE_API UBattleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBattleComponent();

	void SendHitResult(AActor* HitTarget, float DamageAmount, const FHitResult& HitResult, EMonsterSkill SkillType) const;
	void ReceiveHitResult(float DamageAmount, AActor* DamageCauser, const FHitResult& HitResult, EMonsterSkill SkillType) const;

	UPROPERTY(BlueprintAssignable, Category = "Delegate | Battle")
	FOnCharacterHited OnCharacterHited;
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
														 FActorComponentTickFunction* ThisTickFunction) override;
};

