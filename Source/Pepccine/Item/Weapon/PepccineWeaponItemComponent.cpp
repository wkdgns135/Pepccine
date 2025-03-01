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

	if (ProjectileClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		// 프로젝타일을 생성한 주체 설정
		SpawnParams.Instigator = OwnerCharacter;
		SpawnParams.Owner = GetOwner();

		PoolSubSystem->InitializePool(ProjectileClass, 10, SpawnParams);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("투사체 클래스가 설정되어있지 않습니다."));
	}
}

void UPepccineWeaponItemComponent::Fire(const float& WeaponDamage)
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
		const FName SoketName = TEXT("Muzzle");
		if (DoesSocketExist(SoketName))
		{
			// 머즐 위치
			const FVector MuzzleLocation = GetSocketLocation(SoketName);
			// 머즐 회전
			const FRotator MuzzleRotation = GetSocketRotation(SoketName);

			APepccinePoolable* Poolable;
			PoolSubSystem->SpawnFromPool(ProjectileClass, MuzzleLocation, MuzzleRotation, Poolable);
			if (Poolable)
			{
				// 투사체 생성
				if (APepccineProjectile* SpawnedProjectile = Cast<APepccineProjectile>(Poolable))
				{
					SpawnedProjectile->SetOwnerCharacter(OwnerCharacter);
					SpawnedProjectile->SetWeaponDamage(WeaponDamage);

					if (SpawnedProjectile->GetCollisionComp())
					{
						SpawnedProjectile->GetCollisionComp()->IgnoreActorWhenMoving(
							OwnerCharacter->GetInstigator(), true);
						SpawnedProjectile->GetCollisionComp()->IgnoreActorWhenMoving(OwnerCharacter, true);
						SpawnedProjectile->GetCollisionComp()->IgnoreActorWhenMoving(GetOwner(), true);
					}

					// TODO[명관] : 투사체 속도 조정
					SpawnedProjectile->InitProjectile(GetFireDirection(MuzzleLocation), 5000.0f);

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

FVector UPepccineWeaponItemComponent::GetFireDirection(const FVector& MuzzleLocation) const
{
	// 화면 중심 좌표 계산
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ScreenCenter(ViewportSize.X / 2, ViewportSize.Y / 2);

	// 화면 중심에서 월드 방향 가져오기
	FVector WorldLocation, WorldDirection;
	UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetWorld(), 0), ScreenCenter,
	                                         WorldLocation, WorldDirection);

	// Ray 설정
	const FVector Start = WorldLocation;
	const FVector End = Start + (WorldDirection * 10000); // Ray 길이
	FHitResult HitResult;

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, true, 2.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);

	FVector HitLocation;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldDynamic, QueryParams))
	{
		// 충돌한 위치
		HitLocation = HitResult.Location;
		UE_LOG(LogTemp, Warning, TEXT("Camera Ray Hit! : %s"), *HitResult.GetActor()->GetName());
	}
	else
	{
		// 충돌하지 않으면 Ray 끝점
		HitLocation = End;
	}

	return (HitLocation - MuzzleLocation).GetSafeNormal();
}
