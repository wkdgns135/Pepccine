#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UMonsterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMonsterStatComponent();

    FORCEINLINE void IncreaseATK(float amount) { Attack = Attack * amount; }
    FORCEINLINE void IncreaseDEF(float amount) { Defense = Defense * amount; }
    FORCEINLINE void IncreaseSPD(float amount) { Speed = Speed * amount; }
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    float Attack = 10.0f;
    void DecreaseHealth(float Amount);
    void IncreaseHealth(float Amount);
    bool IsDead() const;

protected:
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    float MaxHealth = 100.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
    float CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    float Defense = 5.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    float Speed = 300.0f;
};
