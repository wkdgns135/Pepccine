#include "Item/Weapon/PepccineWeaponItemComponent.h"

#include "Components/SphereComponent.h"
#include "Item/Weapon/PepccineProjectile.h"
#include "GameFramework/Character.h"
#include "Item/Weapon/PepccineWeaponItemData.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectPool/PepccinePoolSubSystem.h"

void UPepccineWeaponItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// 서브 시스템 가져오기
	PoolSubSystem = GetWorld()->GetSubsystem<UPepccinePoolSubSystem>();
}

void UPepccineWeaponItemComponent::InitWeaponComponent(ACharacter* InOwnerCharacter)
{
	if (!InOwnerCharacter)
	{
		return;
	}

	OwnerCharacter = InOwnerCharacter;

	if (!PoolSubSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("PoolSubSystem이 설정되어있지 않습니다."));
		return;
	}
	
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Error, TEXT("투사체 클래스가 설정되어있지 않습니다."));
		return;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// 프로젝타일을 생성한 주체 설정
	SpawnParams.Instigator = OwnerCharacter;
	SpawnParams.Owner = GetOwner();

	PoolSubSystem->InitializePool(ProjectileClass, 10, SpawnParams);
}

void UPepccineWeaponItemComponent::Fire(const float& WeaponDamage, const FVector& ShootDirection)
{
	// 현재 발사 가능 상태가 아니라면 발사 실패(연사 관련)
	if (!bCanFire)
	{
		UE_LOG(LogTemp, Warning, TEXT("발사 준비 중"));
		return;
	}

	// 현재 탄창에 탄약이 없을 경우 발사 실패
	if (EquippedWeaponData->GetWeaponItemStats().MagazineAmmo == 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("탄창에 남은 탄약이 없습니다."));
		return;
	}

	if (const UWorld* World = GetWorld())
	{
		const FName SoketName = GetMuzzleName();
		if (DoesSocketExist(SoketName))
		{
			// 투사체 개수
			const int32 ProjectileCount = EquippedWeaponData->GetWeaponItemStats().ProjectileCount;

			// 확산 각도
			const float SpreadAngle = CalculateSpreadAngle(0.0f, 45.0f);
			// 각도를 라디안으로 변환
			const float SpreadAngleRadians = FMath::DegreesToRadians(SpreadAngle);
			
			for (int32 i = 0; i < ProjectileCount; i++)
			{
				FVector NewShootDirection = FMath::VRandCone(ShootDirection, SpreadAngleRadians);
				
				// 투사체 생성
				APepccinePoolable* Poolable;
				PoolSubSystem->SpawnFromPool(ProjectileClass, GetSocketLocation(SoketName), GetSocketRotation(SoketName), Poolable);
				if (Poolable)
				{
					if (APepccineProjectile* SpawnedProjectile = Cast<APepccineProjectile>(Poolable))
					{
						SpawnedProjectile->SetOwnerCharacter(OwnerCharacter);
						SpawnedProjectile->SetWeaponDamage(WeaponDamage);

						if (SpawnedProjectile->GetCollisionComp())
						{
							SpawnedProjectile->GetCollisionComp()->IgnoreActorWhenMoving(OwnerCharacter, true);
							SpawnedProjectile->GetCollisionComp()->IgnoreActorWhenMoving(GetOwner(), true);
						}
					
						SpawnedProjectile->InitProjectile(NewShootDirection,
														  EquippedWeaponData->GetWeaponItemStats().BulletSpeed,
														  EquippedWeaponData->GetWeaponItemStats().AttackRange);
					}
				}
			}

			if (USoundBase* FireSound = EquippedWeaponData->GetFireSound())
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, OwnerCharacter->GetActorLocation());
			}

			//// Try and play a firing animation if specified
			//if (FireAnimation != nullptr)
			//{
			//	// Get the animation object for the arms mesh
			//	UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
			//	if (AnimInstance != nullptr)
			//	{
			//		AnimInstance->Montage_Play(FireAnimation, 1.f);
			//	}
			//}

			UE_LOG(LogTemp, Warning, TEXT("연사 속도 : %.2f발/초"),
				   EquippedWeaponData->GetWeaponItemStats().FireRate);

			bCanFire = false;
			// 무기 재사용 대기시간 적용
			World->GetTimerManager().SetTimer(EquippedWeaponData->GetFireRateTimerHandle(), this,
											  &UPepccineWeaponItemComponent::CanFire,
											  1.0f / EquippedWeaponData->GetWeaponItemStats().FireRate, false);

			EquippedWeaponData->GetWeaponItemStatsPointer()->MagazineAmmo--;

			UE_LOG(LogTemp, Warning, TEXT("%s 발사! %.0f / %.0f"),
				   *EquippedWeaponData->GetDisplayName(),
				   EquippedWeaponData->GetWeaponItemStats().MagazineAmmo,
				   EquippedWeaponData->GetWeaponItemStats().SpareAmmo);
		}
	}
}

static float MaxSpareAmmo = 999.0f;

bool UPepccineWeaponItemComponent::Reload() const
{
	if (!EquippedWeaponData)
	{
		UE_LOG(LogTemp, Warning, TEXT("장착 무기 데이터가 없습니다."));
		return false;
	}

	FPepccineWeaponStat* EquippedWeaponItemStats = EquippedWeaponData->GetWeaponItemStatsPointer();

	// 탄창 용량
	float& MagazineSize = EquippedWeaponItemStats->MagazineSize;

	if (EquippedWeaponItemStats->SpareAmmo > 0 && EquippedWeaponItemStats->MagazineAmmo == MagazineSize)
	{
		UE_LOG(LogTemp, Warning, TEXT("이미 탄약이 최대 입니다."));
		return false;
	}

	// 주 무기의 경우
	if (EquippedWeaponData->GetWeaponItemType() == EPepccineWeaponItemType::EPWIT_Main)
	{
		if (EquippedWeaponItemStats->SpareAmmo > 0)
		{
			const int32 ModifyValue = MagazineSize - EquippedWeaponItemStats->MagazineAmmo;

			EquippedWeaponItemStats->MagazineAmmo += EquippedWeaponItemStats->SpareAmmo >= ModifyValue
				                                         ? ModifyValue
				                                         : EquippedWeaponItemStats->SpareAmmo;
			EquippedWeaponItemStats->SpareAmmo = FMath::Clamp(EquippedWeaponItemStats->SpareAmmo - ModifyValue, 0.0f,
			                                                  MaxSpareAmmo);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("남은 예비 탄약이 없습니다."));
			return false;
		}
	}
	// 보조 무기의 경우
	else
	{
		EquippedWeaponItemStats->MagazineAmmo = MagazineSize;
	}

	if (USoundBase* ReloadSound = EquippedWeaponData->GetReloadSound())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, OwnerCharacter->GetActorLocation());
	}

	return true;
}

void UPepccineWeaponItemComponent::EquipWeapon(UPepccineWeaponItemData* WeaponItemData, const bool bIsPlayEquipSound)
{
	EquippedWeaponData = WeaponItemData;

	if (bIsPlayEquipSound)
	{
		if (USoundBase* ReloadSound = EquippedWeaponData->GetReloadSound())
		{
			UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, OwnerCharacter->GetActorLocation());
		}
	}
}

float UPepccineWeaponItemComponent::CalculateSpreadAngle(const float BaseAngle, const float MaxAngle) const
{
	// 투사체 개수
	const float ProjectileCount = EquippedWeaponData->GetWeaponItemStats().ProjectileCount;

	// 제곱근 함수를 활용하여 스프레드 각도 계산
	const float SpreadAngle = BaseAngle + (FMath::Sqrt(ProjectileCount) - 1) * (MaxAngle - BaseAngle) / (FMath::Sqrt(100.0f) - 1);
	return FMath::Clamp(SpreadAngle, BaseAngle, MaxAngle);
}
