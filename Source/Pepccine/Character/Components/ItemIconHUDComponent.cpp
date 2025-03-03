#include "ItemIconHUDComponent.h"

UItemIconHUDComponent::UItemIconHUDComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ActiveItemWidget = nullptr;
	WeaponWidget = nullptr;
}

void UItemIconHUDComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ActiveItemWidget) ActiveItemWidget->AddToViewport();
	if (WeaponWidget) WeaponWidget->AddToViewport();
}

void UItemIconHUDComponent::SetWeaponItem(UTexture2D* MainWeaponImage, UTexture2D* SubWeaponImage, const FString& WeaponName, const int32 Ammo, const int32 MaxAmmo, bool bIsMainWeapon)
{
	if (!WeaponWidget) return;
	WeaponWidget->UpdateWeaponUI(MainWeaponImage, SubWeaponImage, WeaponName, Ammo, MaxAmmo, bIsMainWeapon);
}

void UItemIconHUDComponent::SetActiveItem(UTexture2D* ActiveImage, const FString& ActiveName, const FString& ActiveButton, const int32 Cooldown, const int32 MaxCooldown)
{
	if (!ActiveItemWidget) return;
	ActiveItemWidget->UpdateActiveItemUI(ActiveImage, ActiveName, ActiveButton, Cooldown, MaxCooldown);
}

void UItemIconHUDComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

