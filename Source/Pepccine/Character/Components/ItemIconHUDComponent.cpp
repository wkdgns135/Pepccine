#include "ItemIconHUDComponent.h"

#include "Components/TextBlock.h"

UItemIconHUDComponent::UItemIconHUDComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ActiveItemWidget = nullptr;
	WeaponWidget = nullptr;
}

void UItemIconHUDComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ActiveItemWidget)
	{
		ActiveItemWidget->AddToViewport();
		ActiveItemWidget->CoinsText->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (WeaponWidget) WeaponWidget->AddToViewport();
}

void UItemIconHUDComponent::SetWeaponItem(UTexture2D* MainWeaponImage, UTexture2D* SubWeaponImage, const FString& WeaponName, const int32 Ammo, const int32 SpareAmmo, bool bIsMainWeapon)
{
	if (!WeaponWidget) return;
	WeaponWidget->UpdateWeaponUI(MainWeaponImage, SubWeaponImage, WeaponName, Ammo, SpareAmmo, bIsMainWeapon);
}

void UItemIconHUDComponent::SetActiveItem(UTexture2D* ActiveImage, const FString& ActiveName, const FString& ActiveButton, const int32 Cooldown, const int32 MaxCooldown)
{
	if (!ActiveItemWidget) return;
	ActiveItemWidget->UpdateActiveItemUI(ActiveImage, ActiveName, ActiveButton, Cooldown, MaxCooldown);
}

void UItemIconHUDComponent::SetCoins(int Coins)
{
	if (!ActiveItemWidget) return;
	ActiveItemWidget->UpdateCoinUI(Coins);
}

void UItemIconHUDComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

