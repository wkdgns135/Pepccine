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
    TArray<ABaseMonster*> NewSpawnedMonsters;  // 로컬 변수로 선언 (이름 변경)

    if (!MonsterToSpawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("Monster class not set!"));
        return;
    }

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        FVector SpawnLocation = OwnerCharacter->GetActorLocation() + FVector(200.0f, 0.0f, 0.0f);
        FRotator SpawnRotation = FRotator::ZeroRotator;

        for (int i = 0; i < NumberOfMonsters; i++)
        {
            ABaseMonster* SpawnedMonster = GetWorld()->SpawnActor<ABaseMonster>(MonsterToSpawn, SpawnLocation, SpawnRotation);
            if (SpawnedMonster)
            {
                UE_LOG(LogTemp, Warning, TEXT("Monster spawned at: %s"), *SpawnLocation.ToString());
                NewSpawnedMonsters.Add(SpawnedMonster);
            }
        }

        // 블루프린트에서 FadeIn 처리
        FadeInMonsters(NewSpawnedMonsters);
    }
}

