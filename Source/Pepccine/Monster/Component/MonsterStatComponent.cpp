#include "Monster/Component/MonsterStatComponent.h"

UMonsterStatComponent::UMonsterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentHealth = MaxHealth;  // 초기 체력 = 최대 체력
    
}

void UMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("Monster Stat On!"))
}


void UMonsterStatComponent::DecreaseHealth(float Amount)
{
    float FinalDamage = FMath::Max(Amount - Defense, 1.0f);  // 방어력 적용, 최소 데미지 1
    CurrentHealth = FMath::Max(CurrentHealth - FinalDamage, 0.0f);
    UE_LOG(LogTemp, Warning, TEXT("Monster HP: %f"), CurrentHealth);
}

void UMonsterStatComponent::IncreaseHealth(float Amount)
{
    CurrentHealth = FMath::Min(CurrentHealth + Amount, MaxHealth);
}

bool UMonsterStatComponent::IsDead() const
{
    return CurrentHealth <= 0.0f;
}