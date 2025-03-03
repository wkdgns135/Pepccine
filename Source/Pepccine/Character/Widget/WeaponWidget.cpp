#include "Character/Widget/WeaponWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


void UWeaponWidget::UpdateWeaponUI(UTexture2D* MainWeaponImage, UTexture2D* SubWeaponImage, const FString& WeaponName,
                                   const int32 Ammo, const int32 MaxAmmo, bool bIsMainWeapon)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateWeaponUI Called"));
	UE_LOG(LogTemp, Warning, TEXT("WeaponName: %s, Ammo: %d / %d, IsMainWeapon: %s"),
	       *WeaponName, Ammo, MaxAmmo, bIsMainWeapon ? TEXT("True") : TEXT("False"));

	WeaponText->SetText(FText::FromString(WeaponName));
	WeaponAmmo->SetText(FText::FromString(FString::Printf(TEXT("%i/%i"), Ammo, MaxAmmo)));

	if (bIsMainWeapon)
	{
		FrontWeaponImage->SetBrushFromTexture(MainWeaponImage);
		BackWeaponImage->SetBrushFromTexture(SubWeaponImage);
	}
	else
	{
		FrontWeaponImage->SetBrushFromTexture(SubWeaponImage);
		BackWeaponImage->SetBrushFromTexture(MainWeaponImage);
	}
}
