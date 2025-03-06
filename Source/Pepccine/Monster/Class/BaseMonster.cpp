#include "Monster/Class/BaseMonster.h"
#include "MovieSceneObjectBindingID.h"
#include "PepccineGameState.h"
#include "Monster/Component/MonsterStatComponent.h"
#include "Monster/Component/MonsterAttackComponent.h"
#include "Monster/Component/HitReactionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Room/Controller/BaseRoomController.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"
#include "Item/ItemSpawn/PepccineItemSpawnerSubSystem.h"

ABaseMonster::ABaseMonster()
{
	StatComponent = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("StatComponent"));
	AttackComponent = CreateDefaultSubobject<UMonsterAttackComponent>(TEXT("AttackComponent"));
	HitReactionComponent = CreateDefaultSubobject<UHitReactionComponent>(TEXT("HitReactionComponent"));
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BaseAudio"));
	HealthBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	
	HealthBarWidgetComp->SetupAttachment(RootComponent);
	HealthBarWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComp->SetDrawSize(FVector2D(77.0f, 7.0f));
	HealthBarWidgetComp->SetRelativeLocation(FVector(0, 0, 100.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HealthWidgetClass(TEXT("/Game/Pepccine/Monster/BP_MonsterHealthWidget"));
	if (HealthWidgetClass.Succeeded())
	{
		HealthBarWidgetComp->SetWidgetClass(HealthWidgetClass.Class);
	}

	AudioComponent->SetupAttachment(RootComponent);
}

void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();

	if (AttackComponent)
	{
		AttackComponent->OnCharacterHited.AddDynamic(this, &ABaseMonster::OnHitReceived);
	}

	UE_LOG(LogTemp, Warning, TEXT("Monster Spawned!"));

	InitializeHealthBar();

	if (BaseSound)
	{
		AudioComponent->SetSound(BaseSound);
		AudioComponent->SetBoolParameter(FName("Loop"), true);
		AudioComponent->Play();
	}
}

void ABaseMonster::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// [장훈] 몬스터가 Destroy 될때 방의 몬스터 카운트 감소
	if (APepccineGameState* PepccineGameState = Cast<APepccineGameState>(GetWorld()->GetGameState()))
	{
		if (ABaseRoomController* RoomController = PepccineGameState->GetRoomController())
		{
			RoomController->DecreaseMonsterCount();
		}
	}

	UPepccineItemSpawnerSubSystem* TestSub = GetWorld()->GetGameInstance()->GetSubsystem<UPepccineItemSpawnerSubSystem>();
	UPepccineItemDataBase* Test = TestSub->GetRandomItemFromWeightDataAsset(SpawnWeightData);
	TestSub->SpawnItem(GetActorLocation(), Test, false);
}

void ABaseMonster::InitializeHealthBar()
{
	if (HealthBarWidgetComp)
	{
		if (UUserWidget* Widget = HealthBarWidgetComp->GetWidget())
		{
			HealthInstance = Cast<UMonsterHealthWidget>(Widget);
			if (HealthInstance)
			{
				UE_LOG(LogTemp, Warning, TEXT("HealthInstance"));
				HealthInstance->UpdateHealthBar(0.5f);
				HealthBarWidgetComp->SetHiddenInGame(true);
			}
		}
	}
}

void ABaseMonster::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (HealthInstance)
	{
		HealthInstance->UpdateHealthBar(CurrentHealth / MaxHealth);
	}
}

void ABaseMonster::OnHitReceived(AActor* DamageCauser, float DamageAmount, const FHitResult& HitResult, EMonsterSkill SkillType)
{
	UE_LOG(LogTemp, Warning, TEXT("Monster hit! Damage: %f, Hit Location: %s, BoneName: %s"), DamageAmount,
	       *HitResult.Location.ToString(), *HitResult.BoneName.ToString());
	if (StatComponent)
	{
		if (HitResult.BoneName == FName(TEXT("Head")))
		{
			StatComponent->DecreaseHealth(DamageAmount * 2);
		}
		else
		{
			StatComponent->DecreaseHealth(DamageAmount);
		}

		FVector HitDirection = DamageCauser
			                       ? (GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal()
			                       : FVector::ZeroVector;

		if (HitReactionComponent)
		{
			HitReactionComponent->HandleHitReaction(DamageAmount);
		}

		if (StatComponent->IsDead())
		{
			UE_LOG(LogTemp, Warning, TEXT("Monster %s has died!"), *GetName());
			Die();
		}
	}
}

void ABaseMonster::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("Monster Die!"));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	GetMesh()->Stop();
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	SetLifeSpan(3.0f);
}
