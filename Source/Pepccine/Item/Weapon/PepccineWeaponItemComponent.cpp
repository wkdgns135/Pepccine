#include "Item/Weapon/PepccineWeaponItemComponent.h"
#include "Item/Weapon/PepccineProjectile.h"
#include "GameFramework/Character.h"
#include "Item/Weapon/PepccineWeaponItemData.h"
#include "Kismet/GameplayStatics.h"
#include "Item/Weapon/WeaponStatModifier.h"

void UPepccineWeaponItemComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UPepccineWeaponItemComponent::Fire() const
{
	// 현재 탄창에 탄약이 없을 경우 발사 실패
	if (EquippedWeaponData->GetWeaponItemStats().MagazineAmmo == 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("탄창에 남은 탄약이 없습니다."));
		return false;
	}

	if (TSubclassOf<APepccineProjectile> Projectile = EquippedWeaponData->GetProjectileClass())
	{
		if (UWorld* World = GetWorld())
		{
			const FName SoketName = TEXT("Muzzle");
			if (DoesSocketExist(SoketName))
			{
				// 카메라 방향으로 바라보기
				RotateToCamera();

				const FRotator MuzzleRotation = GetSocketRotation(SoketName);
				const FVector MuzzleLocation = GetSocketLocation(SoketName);

				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride =
					ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// 투사체 생성
				APepccineProjectile* SpawnedProjectile = World->SpawnActor<APepccineProjectile>(
					Projectile, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
				SpawnedProjectile->SetProjectileVelocity(GetFireDirection(MuzzleLocation));

				//// Try and play the sound if specified
				//if (FireSound != nullptr)
				//{
				//	UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
				//}

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

				return true;
			}
		}
	}

	return false;
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
			EquippedWeaponItemStats->SpareAmmo = FMath::Clamp(EquippedWeaponItemStats->SpareAmmo - ModifyValue, 0.0f, MaxSpareAmmo);
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

	return true;
}

void UPepccineWeaponItemComponent::EquipWeapon(UPepccineWeaponItemData* WeaponItemData)
{
	EquippedWeaponData = WeaponItemData;
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

	FVector HitLocation;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	{
		HitLocation = HitResult.Location; // 충돌한 위치
	}
	else
	{
		HitLocation = End; // 충돌하지 않으면 Ray 끝점
	}

	return (HitLocation - MuzzleLocation).GetSafeNormal();
}

void UPepccineWeaponItemComponent::RotateToCamera() const
{
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("캐릭터가 지정되지 않았습니다."));
	}

	if (const AController* Controller = OwnerCharacter->GetController())
	{
		FRotator CameraRotation = Controller->GetControlRotation();
		CameraRotation.Pitch = 0.0f;
		CameraRotation.Roll = 0.0f;

		OwnerCharacter->SetActorRotation(CameraRotation);
	}
}
