#include "Monster/Component/SpawnSkillComponent.h"
#include "Monster/Class/BaseMonster.h"

void USpawnSkillComponent::ActivateSkill()
{
    if (!IsCooldownOver())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cooldown not over yet!"));
        return;
    }

    StartCooldown();
    PlaySkillMontage();
    SpawnMonster();
}

void USpawnSkillComponent::SpawnMonster()
{
    if (!MonsterToSpawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("Monster class not set!"));
        return;
    }

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        FVector SpawnLocation = OwnerCharacter->GetActorLocation() + FVector(200.0f, 0.0f, 0.0f); // 소환 위치를 캐릭터 앞쪽으로 설정
        FRotator SpawnRotation = FRotator::ZeroRotator;

        for (int i = 0; i < NumberOfMonsters; i++)
        {
            // 소환
            ABaseMonster* SpawnedMonster = GetWorld()->SpawnActor<ABaseMonster>(MonsterToSpawn, SpawnLocation, SpawnRotation);
            if (SpawnedMonster)
            {
                UE_LOG(LogTemp, Warning, TEXT("Monster spawned at: %s"), *SpawnLocation.ToString());
                FadeInMonster(SpawnedMonster);
            }
        }
    }
}