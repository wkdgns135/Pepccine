#include "Character/Widget/WeaponWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


void UWeaponWidget::UpdateWeaponUI(UTexture2D* MainWeaponImage, UTexture2D* SubWeaponImage, const FString& WeaponName,
                                   const int32 Ammo, const int32 SpareAmmo, bool bIsMainWeapon)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateWeaponUI Called"));
	UE_LOG(LogTemp, Warning, TEXT("WeaponName: %s, Ammo: %d / %d, IsMainWeapon: %s"),
	       *WeaponName, Ammo, SpareAmmo, bIsMainWeapon ? TEXT("True") : TEXT("False"));

	if (!WeaponText)
	{
		WeaponText->SetText(FText::FromString(WeaponName));
	}

	if (bIsMainWeapon)
	{
		FrontWeaponImage->SetBrushFromTexture(MainWeaponImage);
		BackWeaponImage->SetBrushFromTexture(SubWeaponImage);
		WeaponAmmo->SetText(FText::FromString(FString::Printf(TEXT("%i/%i"), Ammo, SpareAmmo)));
	}
	else
	{
		FrontWeaponImage->SetBrushFromTexture(SubWeaponImage);
		BackWeaponImage->SetBrushFromTexture(MainWeaponImage);
		WeaponAmmo->SetText(FText::FromString(FString::Printf(TEXT("%i/∞"), Ammo)));
	}
}
