#include "Monster/Component/BerserkModeComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Actor.h"
#include "Monster/Component/MonsterStatComponent.h"

UBerserkModeComponent::UBerserkModeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UBerserkModeComponent::BeginPlay()
{
	Super::BeginPlay();
    bIsBerserkMode = false;
}

void UBerserkModeComponent::EnterBerserkMode()
{
    if (bIsBerserkMode)
    {
        return;
    }

    bIsBerserkMode = true;

    // 스텟 증가 로직
    AActor* Owner = GetOwner();
    if (Owner)
    {
        UMonsterStatComponent* StatComp = Owner->FindComponentByClass<UMonsterStatComponent>();
        if (StatComp)
        {
            StatComp->IncreaseATK(AttackMultiplier);
            StatComp->IncreaseDEF(DefenseMultiplier);
            StatComp->IncreaseSPD(SpeedMultiplier);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("BerserkModeComponent: MonsterStatComponent not found!"));
        }
    }

    PlayModeChangeMontage();
}

void UBerserkModeComponent::PlayModeChangeMontage()
{
    if (ModeChangeMontage && GetOwner())
    {
        AActor* Owner = GetOwner();
        USkeletalMeshComponent* MeshComp = Owner->FindComponentByClass<USkeletalMeshComponent>();

        if (MeshComp && MeshComp->GetAnimInstance())
        {
            UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
            AnimInstance->Montage_Play(ModeChangeMontage);
        }
    }
}