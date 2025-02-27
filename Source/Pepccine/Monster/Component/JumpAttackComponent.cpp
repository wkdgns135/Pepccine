#include "Monster/Component/JumpAttackComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h" // UGameplayStatics 필요

void UJumpAttackComponent::ActivateSkill()
{
    if (!IsCooldownOver())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cooldown not over yet!"));
        return;
    }

    StartCooldown();
    PlaySkillMontage();

    ACharacter* OwnerMonster = Cast<ACharacter>(GetOwner());
    if (OwnerMonster)
    {

        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            UE_LOG(LogTemp, Warning, TEXT("PlayerController found"));

            APawn* PlayerPawn = PlayerController->GetPawn();
            if (PlayerPawn)
            {
                // 플레이어와 몬스터의 현재 위치를 계산
                FVector PlayerLocation = PlayerPawn->GetActorLocation();
                FVector MonsterLocation = OwnerMonster->GetActorLocation();

                // 목표 위치까지의 차이 계산
                FVector DirectionToPlayer = (PlayerLocation - MonsterLocation).GetSafeNormal();

                // 점프할 방향과 세기 계산
                FVector LaunchDirection = DirectionToPlayer * JumpPower;

                // 점프 높이도 설정 (Z축)
                LaunchDirection.Z = JumpHeight;

                // 점프 적용
                OwnerMonster->LaunchCharacter(LaunchDirection, true, true);

                UE_LOG(LogTemp, Warning, TEXT("Jump launched towards player at: %s"), *PlayerLocation.ToString());
            }
            
        }
        
    }
    
}
